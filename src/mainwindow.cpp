#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QCheckBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_message(new Message)
{
    setupUI();
    createMenus();
    setWindowTitle(tr("Encodage de message dans un parachute"));
}

void MainWindow::setupUI() {
    // Widget central
    auto centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    
    // Layout principal
    auto mainLayout = new QVBoxLayout(centralWidget);
    
    // Zone de saisie du message
    auto messageLayout = new QHBoxLayout;
    messageLayout->addWidget(new QLabel(tr("Message:")));
    m_messageEdit = new QLineEdit;
    m_messageEdit->setText(m_message->getText());
    connect(m_messageEdit, &QLineEdit::textChanged, this, &MainWindow::updateMessage);
    messageLayout->addWidget(m_messageEdit);
    mainLayout->addLayout(messageLayout);
    
    // Contrôles
    auto controlsLayout = new QHBoxLayout;
    
    // Nombre de pistes
    auto ringsLayout = new QVBoxLayout;
    ringsLayout->addWidget(new QLabel(tr("Nombre de pistes:")));
    m_ringsSpinBox = new QSpinBox;
    m_ringsSpinBox->setRange(1, 10);
    m_ringsSpinBox->setValue(m_message->getRings());
    connect(m_ringsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::updateRings);
    ringsLayout->addWidget(m_ringsSpinBox);
    controlsLayout->addLayout(ringsLayout);
    
    // Nombre de secteurs
    auto sectorsLayout = new QVBoxLayout;
    sectorsLayout->addWidget(new QLabel(tr("Nombre de secteurs:")));
    m_sectorsSlider = new QSlider(Qt::Horizontal);
    m_sectorsSlider->setRange(4, 40);
    m_sectorsSlider->setSingleStep(4);
    m_sectorsSlider->setValue(m_message->getSectors());
    connect(m_sectorsSlider, &QSlider::valueChanged,
            this, &MainWindow::updateSectors);
    sectorsLayout->addWidget(m_sectorsSlider);
    controlsLayout->addLayout(sectorsLayout);
    
    mainLayout->addLayout(controlsLayout);
    
    // Vue binaire
    auto viewLayout = new QHBoxLayout;
    auto binaryViewCheck = new QCheckBox(tr("Vue binaire"));
    connect(binaryViewCheck, &QCheckBox::toggled,
            this, &MainWindow::toggleBinaryView);
    viewLayout->addWidget(binaryViewCheck);
    mainLayout->addLayout(viewLayout);
    
    // Vue du parachute
    m_parachuteView = new ParachuteView;
    m_parachuteView->setMessage(m_message);
    mainLayout->addWidget(m_parachuteView);
}

void MainWindow::createMenus() {
    auto fileMenu = menuBar()->addMenu(tr("&Fichier"));
    
    auto quitAction = new QAction(tr("&Quitter"), this);
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(quitAction);
}

void MainWindow::updateMessage() {
    m_message->setText(m_messageEdit->text());
    m_parachuteView->update();
}

void MainWindow::updateRings(int value) {
    m_message->setRings(value);
    m_parachuteView->update();
}

void MainWindow::updateSectors(int value) {
    m_message->setSectors(value);
    m_parachuteView->update();
}

void MainWindow::toggleBinaryView(bool enabled) {
    m_parachuteView->setBinaryViewMode(enabled);
} 