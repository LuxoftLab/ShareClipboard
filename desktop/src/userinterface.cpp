#include "userinterface.h"

UserInterface::UserInterface(ClientList *peer_list, QObject *parent) :
    QObject(parent),
    m_tray(0),
    m_main_menu(0),
    m_status_menu(0),
    m_peers_menu(0),
    m_enable_action(0),
    m_disable_action(0),
    m_show_log_action(0),
    m_peer_list(peer_list),
    m_log_view(0)
{
    connect(m_peer_list,&ClientList::peerListChanged,this,&UserInterface::onPeerListChanged);

    m_tray = new QSystemTrayIcon(QIcon(":/img/clipboard-big.png"));
    m_main_menu = new QMenu();
    m_tray->setContextMenu(m_main_menu);

    m_status_menu = m_main_menu->addMenu(tr("Status"));
    QActionGroup * group = new QActionGroup(m_status_menu);

    m_enable_action = m_status_menu->addAction(tr("Enabled"));
    m_enable_action->setCheckable(true);
    m_enable_action->setChecked(true);
    m_enable_action->setActionGroup(group);
    connect(m_enable_action,&QAction::triggered,this,&UserInterface::onEnabledTriggered);

    m_disable_action = m_status_menu->addAction(tr("Disabled"));
    m_disable_action->setCheckable(true);
    m_disable_action->setActionGroup(group);
    connect(m_disable_action,&QAction::triggered,this,&UserInterface::onDisabledTriggered);

    m_log_view = new LogView(0);
    m_show_log_action = m_main_menu->addAction(tr("Show log..."));
    connect(m_show_log_action,&QAction::triggered,m_log_view,&LogView::show);

    m_peers_menu = m_main_menu->addMenu(tr("Peer list"));

    m_tray->show();
}

UserInterface::~UserInterface()
{
    delete m_tray;
    delete m_log_view;
}

void UserInterface::onEnabledTriggered(bool checked)
{
    if(checked)
    {
        emit enable();
        Logger::instance()<<TimeStamp()<<"Me: enable service"<<Flush();
    }
}

void UserInterface::onDisabledTriggered(bool checked)
{
    if(checked)
    {
        emit disable();
        Logger::instance()<<TimeStamp()<<"Me: disable service"<<Flush();
    }
}

void UserInterface::onPeerListChanged(QStringList peers)
{
    m_peers_menu->clear();
    foreach (QString peer, peers) {
        m_peers_menu->addAction(peer);
    }
}
