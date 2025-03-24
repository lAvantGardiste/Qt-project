#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QCheckBox>
#include <QGroupBox>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_message(new Message)
{
    setupUI();
    createMenus();
    setWindowTitle(tr("Encodage de message dans un parachute"));

    // Style minimal pour ne pas interférer avec les flèches natives
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: white;
        }
        QLabel, QCheckBox, QGroupBox {
            color: black;
        }
        QLineEdit, QSpinBox {
            background-color: white;
            color: black;
            border: 1px solid #999;
            min-width: 80px;
        }
        QGroupBox {
            border: 1px solid #999;
            margin-top: 6px;
            padding-top: 10px;
        }
        QGroupBox::title {
            color: black;
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 3px;
        }
        QSlider::groove:horizontal {
            border: 1px solid #999;
            height: 8px;
            background: white;
            margin: 2px 0;
        }
        QSlider::handle:horizontal {
            background: #999;
            border: 1px solid #5c5c5c;
            width: 18px;
            margin: -2px 0;
            border-radius: 3px;
        }
    )");
}

void MainWindow::setupUI() {
    // Widget central
    auto centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    
    // Layout principal
    auto mainLayout = new QVBoxLayout(centralWidget);
    
    // Zone de saisie du message
    auto messageGroup = new QGroupBox(tr("Message"));
    auto messageLayout = new QHBoxLayout(messageGroup);
    m_messageEdit = new QLineEdit;
    m_messageEdit->setText(m_message->getText());
    m_messageEdit->setPlaceholderText(tr("Entrez votre message ici"));
    connect(m_messageEdit, &QLineEdit::textChanged, this, &MainWindow::updateMessage);
    messageLayout->addWidget(m_messageEdit);
    mainLayout->addWidget(messageGroup);
    
    // Contrôles
    auto controlsGroup = new QGroupBox(tr("Paramètres"));
    auto controlsLayout = new QHBoxLayout(controlsGroup);
    
    // Nombre de pistes
    auto ringsLayout = new QVBoxLayout;
    ringsLayout->addWidget(new QLabel(tr("Nombre de pistes:")));
    
    // SpinBox pour les pistes
    m_ringsSpinBox = new QSpinBox;
    m_ringsSpinBox->setRange(1, 10);
    m_ringsSpinBox->setValue(m_message->getRings());
    m_ringsSpinBox->setButtonSymbols(QSpinBox::UpDownArrows);
    m_ringsSpinBox->setAlignment(Qt::AlignCenter);
    connect(m_ringsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::updateRings);
    ringsLayout->addWidget(m_ringsSpinBox);
    
    // Slider pour les pistes
    m_ringsSlider = new QSlider(Qt::Horizontal);
    m_ringsSlider->setRange(1, 10);
    m_ringsSlider->setValue(m_message->getRings());
    connect(m_ringsSlider, &QSlider::valueChanged,
            m_ringsSpinBox, &QSpinBox::setValue);
    connect(m_ringsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            m_ringsSlider, &QSlider::setValue);
    ringsLayout->addWidget(m_ringsSlider);
    
    controlsLayout->addLayout(ringsLayout);
    
    // Nombre de secteurs
    auto sectorsLayout = new QVBoxLayout;
    sectorsLayout->addWidget(new QLabel(tr("Nombre de secteurs:")));
    
    // SpinBox pour les secteurs
    m_sectorsSpinBox = new QSpinBox;
    m_sectorsSpinBox->setRange(1, 40);
    m_sectorsSpinBox->setValue(m_message->getSectors());
    m_sectorsSpinBox->setButtonSymbols(QSpinBox::UpDownArrows);
    m_sectorsSpinBox->setAlignment(Qt::AlignCenter);
    connect(m_sectorsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::updateSectors);
    sectorsLayout->addWidget(m_sectorsSpinBox);
    
    // Slider pour les secteurs
    m_sectorsSlider = new QSlider(Qt::Horizontal);
    m_sectorsSlider->setRange(1, 40);
    m_sectorsSlider->setValue(m_message->getSectors());
    connect(m_sectorsSlider, &QSlider::valueChanged,
            m_sectorsSpinBox, &QSpinBox::setValue);
    connect(m_sectorsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            m_sectorsSlider, &QSlider::setValue);
    sectorsLayout->addWidget(m_sectorsSlider);
    
    controlsLayout->addLayout(sectorsLayout);
    mainLayout->addWidget(controlsGroup);
    
    // Vue binaire
    auto viewGroup = new QGroupBox(tr("Affichage"));
    auto viewLayout = new QHBoxLayout(viewGroup);
    auto binaryViewCheck = new QCheckBox(tr("Vue binaire"));
    connect(binaryViewCheck, &QCheckBox::toggled,
            this, &MainWindow::toggleBinaryView);
    viewLayout->addWidget(binaryViewCheck);
    mainLayout->addWidget(viewGroup);
    
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