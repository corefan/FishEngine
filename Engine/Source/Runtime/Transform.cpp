#include "Transform.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"
#include "Common.hpp"

namespace FishEngine
{
    Transform::Transform() : m_localPosition(0, 0, 0), m_localScale(1, 1, 1), m_localRotation(0, 0, 0, 1)
    {

    }

    Transform::~Transform()
    {
		m_children.clear();
        //Debug::Log("~Transform");
        SetParent(nullptr); // remove from parent
        //for (auto child : m_children) {
        //}
    }

    void Transform::UpdateMatrix() const
    {
        if (!m_isDirty)
            return;
#if 1
		m_localToWorldMatrix.SetTRS(m_localPosition, m_localRotation, m_localScale);
		if (!m_parent.expired()) {
			m_localToWorldMatrix = m_parent.lock()->localToWorldMatrix() * m_localToWorldMatrix;
		}
		m_worldToLocalMatrix = m_localToWorldMatrix.inverse();
#else
		// TODO this version is not right, take a look to see where the bug is.
		// maybe in the TRS
		Matrix4x4::TRS(m_localPosition, m_localRotation, m_localScale, m_localToWorldMatrix, m_worldToLocalMatrix);
		if (!m_parent.expired()) {
			m_localToWorldMatrix = m_parent.lock()->localToWorldMatrix() * m_localToWorldMatrix;
			m_worldToLocalMatrix = m_worldToLocalMatrix * m_parent.lock()->worldToLocalMatrix();
		}
#endif
        m_isDirty = false;
    }

    void Transform::UpdateFast() const
    {
        //m_localEulerAngles = m_localRotation.eulerAngles();
        //m_localToWorldMatrix.SetTRS(m_localPosition, m_localRotation, m_localScale);
        Matrix4x4::TRS(m_localPosition, m_localRotation, m_localScale, m_localToWorldMatrix, m_worldToLocalMatrix);
        if (!m_parent.expired()) {
            m_localToWorldMatrix = m_parent.lock()->m_localToWorldMatrix * m_localToWorldMatrix;
            m_worldToLocalMatrix = m_worldToLocalMatrix * m_parent.lock()->m_worldToLocalMatrix;
        }
        //m_worldToLocalMatrix = m_localToWorldMatrix.inverse();
        //m_rotation = m_localToWorldMatrix.ToRotation();
        //m_isDirty = false;
    }

    void Transform::LookAt(const Vector3& target, const Vector3& worldUp /*= Vector3(0, 1, 0)*/)
    {
        auto m = Matrix4x4::LookAt(position(), target, worldUp);
        setRotation(m.inverse().ToRotation());
        //m_localRotation = m.inverse().ToRotation();
        //MakeDirty();
    }

    Vector3 Transform::TransformDirection(const Vector3& direction) const
    {
        UpdateMatrix();
        return m_localToWorldMatrix.MultiplyVector(direction);
    }

    Vector3 FishEngine::Transform::InverseTransformDirection(const Vector3& direction) const
    {
        UpdateMatrix();
        return m_worldToLocalMatrix.MultiplyVector(direction);
    }
    
    Bounds Transform::TransformBounds(const Bounds& bounds) const
    {
        //Update();
        auto l2w = localToWorldMatrix();
        auto center = bounds.center();
        auto extents = bounds.extents();
        float weights[] = { 1, 1, 1,  1, 1, -1,  1, -1, 1,  -1, 1, 1,
            1, -1, -1,  -1, 1, -1,  -1, -1, 1,  -1, -1, -1};
        Bounds result;
        for (int i = 0; i < 8; ++i)
        {
            Vector3 corner = center + extents * Vector3(weights+i*3);
            corner = l2w.MultiplyPoint(corner);
            result.Encapsulate(corner);
        }
        return result;
    }
    
    Bounds Transform::InverseTransformBounds(const Bounds& bounds) const
    {
        // TODO: hack for empty scene
        if (bounds.IsEmpty())
            return bounds;
        auto w2l = worldToLocalMatrix();
        auto center = bounds.center();
        auto extents = bounds.extents();
        float weights[] = { 1, 1, 1,  1, 1, -1,  1, -1, 1,  -1, 1, 1,
            1, -1, -1,  -1, 1, -1,  -1, -1, 1,  -1, -1, -1};
        Bounds result;
        for (int i = 0; i < 8; ++i)
        {
            Vector3 corner = center + extents * Vector3(weights+i*3);
            corner = w2l.MultiplyPoint(corner);
            result.Encapsulate(corner);
        }
        return result;
    }

    void Transform::Translate(const Vector3& translation, Space relativeTo /*= Space::Self*/)
    {
        if (relativeTo == Space::World) {
            setPosition(position() + translation);
        }
        else {
            setPosition(position() + TransformDirection(translation));
        }
        MakeDirty();
    }

    //void Transform::Rotate(Vector3 eulerAngles, Space relativeTo /*= Space::Self*/)
    //{
    //    Quaternion lhs(glm::radians(eulerAngles));
    //    if (Space::Self == relativeTo) {
    //        //auto r = this->rotation();
    //        //m_localRotation = glm::inverse(r) * lhs * r * m_localRotation;
    //        m_localRotation = lhs * m_localRotation;
    //    }
    //    else {
    //        //m_localRotation = lhs * m_localRotation;
    //        lhs * rotation();
    //        // TODO
    //    }
    //}

    void Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
    {
        // step1: update position
        auto vector = this->position();
        auto rotation = Quaternion::AngleAxis(angle, axis);
        Vector3 vector2 = vector - point;
        vector2 = rotation * vector2;
        vector = point + vector2;
        setPosition(vector);

        // step2: update rotation
        m_localRotation = rotation * m_localRotation;
        //RotateAroundInternal(axis, angle);
        MakeDirty();
    }


    void Transform::SetParent(TransformPtr const & parent, bool worldPositionStays)
    {
		auto old_parent = m_parent.lock();
		if (parent == old_parent)
		{
            return;
        }
		
		// new parent can not be child of this
		auto p = parent;
		while (p != nullptr)
		{
			if (p.get() == this)
			{
				Debug::LogError("%s", "new parent can not be child of this");
				return;
			}
			p = p->parent();
		}
		
		
        // remove from old parent
		if (old_parent != nullptr)
		{
            //p->m_children.remove(this);
			// TODO: remove first, not remove all
			old_parent->m_children.remove_if([this](std::weak_ptr<Transform> c) {
                return c.lock().get() == this;
            });
        }
		
		// old_parent.localToWorld * old_localToWorld = new_parent.localToWorld * new_localToWorld
		// ==> new_localToWorld = new_parent.worldToLocal * old_parent.localToWorld * old_localToWorld

        m_parent = parent;
		if (parent != nullptr)
		{
			parent->m_children.push_back(gameObject()->transform());
        }
		
		if ( worldPositionStays )
		{
			Matrix4x4 mat = Matrix4x4::TRS(m_localPosition, m_localRotation, m_localScale);
			if (old_parent != nullptr)
				mat = old_parent->localToWorldMatrix() * mat;
			if (parent != nullptr)
				mat = parent->worldToLocalMatrix() * localToWorldMatrix();
			Matrix4x4::Decompose(mat, &m_localPosition, &m_localRotation, &m_localScale);
		}
		//UpdateMatrix();
		MakeDirty();
    }

    //std::shared_ptr<Transform>
    //Transform::Find(const std::string& name) const
    //{
    //    auto pos = name.find('/');
    //    if (pos == std::string::npos) {
    //        for (auto& c : m_children) {
    //            const auto& t = c.lock();
    //            if (t->name() == name) {
    //                return t;
    //            }
    //        }
    //        return nullptr;
    //    }
    //    
    //    auto t = gameObject()->transform();
    //    const auto& splits = split(name, "/");
    //    for (auto& n : splits) {
    //        t = t->Find(n);
    //        if (t == nullptr) {
    //            return nullptr;
    //        }
    //    }
    //    return nullptr;
    //}

    TransformPtr Transform::GetChild(const size_t index)
    {
        if (index >= m_children.size()) {
            Debug::LogWarning("%s %d %s index out of range", __FILE__, __LINE__, __FUNCTION__);
            return nullptr;
        }

        auto p = m_children.begin();
        for (size_t i = 0; i < index; ++i) {
            p++;
        }
        return p->lock();
    }


    // bool Transform::dirtyInHierarchy() const
    // {
    //     if (!m_isDirty && !m_parent.expired()) // not dirty and has a parent
    //     {
    //         return m_parent.lock()->dirtyInHierarchy();
    //     }
    //     return m_isDirty;
    // }


    void FishEngine::Transform::MakeDirty() const
    {
        if (!m_isDirty)
        {
            for (auto& c : m_children)
            {
                c.lock()->MakeDirty();
            }
            m_isDirty = true;
        }
    }
}
