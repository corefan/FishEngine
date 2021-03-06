#include "InspectorWidget.hpp"

#include <QTimer>
#include <QMenu>
#include <QCursor>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include "UI/UIGameObjectHeader.hpp"
#include "UI/UIMaterialHeader.hpp"
#include "UI/UIAssetHeader.hpp"

#include "TextureImporter.hpp"
#include "ModelImporter.hpp"
#include "Selection.hpp"
#include "Inspector.hpp"

InspectorWidget::InspectorWidget(QWidget *parent) 
	: QWidget(parent)
{
	//m_menu = new QMenu(this);
	//m_menu->addAction("Rigidbody");

    QVBoxLayout * rootLayout = new QVBoxLayout(this);
    rootLayout->setMargin(0);
	//rootLayout->setContentsMargins(0, 0, 4, 0);
    rootLayout->setSpacing(0);
    setLayout(rootLayout);
    m_treeWidget = new QTreeWidget();
    m_treeWidget->setHeaderHidden(true);
    m_treeWidget->setIndentation(12);
    m_treeWidget->setSelectionMode(QAbstractItemView::NoSelection);

    m_gameObjectHeader = new UIGameObjectHeader();
    m_materialHeader = new UIMaterialHeader();
    m_assetHeader = new UIAssetHeader();
    rootLayout->addWidget(m_gameObjectHeader);
    rootLayout->addWidget(m_materialHeader);
    rootLayout->addWidget(m_assetHeader);
    m_gameObjectHeader->setHidden(true);
    m_materialHeader->setHidden(true);
    m_assetHeader->setHidden(true);
    rootLayout->addWidget(m_treeWidget);

	auto timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &InspectorWidget::Update);
	timer->start(1000 / 15.0f); // 15 fps
}

void InspectorWidget::Bind(std::shared_ptr<FishEngine::Object> obj)
{
    if ( !m_gameObjectHeader->isHidden() )
        m_gameObjectHeader->setHidden(true);
    if ( ! m_materialHeader->isHidden() )
        m_materialHeader->setHidden(true);
    if ( m_assetHeader->isHidden() )
        m_assetHeader->setHidden(false);
}

void InspectorWidget::Bind(std::shared_ptr<FishEngine::GameObject> go)
{
    if ( m_gameObjectHeader->isHidden() )
        m_gameObjectHeader->setHidden(false);
    if ( ! m_materialHeader->isHidden() )
        m_materialHeader->setHidden(true);
    if ( ! m_assetHeader->isHidden() )
        m_assetHeader->setHidden(true);

    m_gameObjectHeader->Bind(go);
}

void InspectorWidget::Bind(std::shared_ptr<FishEditor::TextureImporter> importer)
{
	if (!m_gameObjectHeader->isHidden())
		m_gameObjectHeader->setHidden(true);
	if (!m_materialHeader->isHidden())
		m_materialHeader->setHidden(true);
	if (m_assetHeader->isHidden())
		m_assetHeader->setHidden(false);

	m_assetHeader->CheckUpdate(importer->name());
}

void InspectorWidget::Bind(std::shared_ptr<FishEditor::ModelImporter> importer)
{
	if (!m_gameObjectHeader->isHidden())
		m_gameObjectHeader->setHidden(true);
	if (!m_materialHeader->isHidden())
		m_materialHeader->setHidden(true);
	if (m_assetHeader->isHidden())
		m_assetHeader->setHidden(false);

	m_assetHeader->CheckUpdate(importer->name());
}

void InspectorWidget::Update()
{
	auto go = FishEditor::Selection::activeGameObject();
	if (go != nullptr)
	{
		FishEditor::Inspector::Bind(go);
	}
	else
	{
		auto object = FishEditor::Selection::activeObject();
		if (object != nullptr)
			FishEditor::Inspector::Bind(object);
		else
			FishEditor::Inspector::HideAll();
	}
}

//std::string InspectorWidget::ShowAddComponentMenu()
//{
//	auto action = m_menu->exec(QCursor::pos());
//	if (action == nullptr)
//		return "";
//	return action->text().toStdString();
//}
