#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QCheckBox>
#include <QGroupBox>
#include <QApplication>
#include <QStyleFactory>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_message(new Message)
{
    setupUI();
    createMenus();
    setWindowTitle(tr("Encodage de message dans un parachute"));

    // Style avec tous les indices de débogage
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

MainWindow::~MainWindow()
{
    delete m_message;
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
    m_ringsSpinBox->setButtonSymbols(QSpinBox::PlusMinus);  // Utiliser PlusMinus au lieu de UpDownArrows
    m_ringsSpinBox->setAlignment(Qt::AlignCenter);
    m_ringsSpinBox->setFixedWidth(100);
    m_ringsSpinBox->setStyle(QStyleFactory::create("Fusion"));  // Utiliser le style Fusion
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
    m_sectorsSpinBox->setButtonSymbols(QSpinBox::PlusMinus);  // Utiliser PlusMinus au lieu de UpDownArrows
    m_sectorsSpinBox->setAlignment(Qt::AlignCenter);
    m_sectorsSpinBox->setFixedWidth(100);
    m_sectorsSpinBox->setStyle(QStyleFactory::create("Fusion"));  // Utiliser le style Fusion
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
    
    auto openAction = new QAction(tr("&Ouvrir..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAction);
    
    auto saveAction = new QAction(tr("&Enregistrer..."), this);
    saveAction->setShortcuts(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAction);
    
    fileMenu->addSeparator();
    
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

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Ouvrir un fichier de configuration"), "",
        tr("Fichiers de parachute (*.para);;Tous les fichiers (*)"));
        
    if (fileName.isEmpty())
        return;
        
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Erreur de lecture"),
                             tr("Impossible d'ouvrir le fichier %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QString message = in.readLine();
    int rings = in.readLine().toInt();
    int sectors = in.readLine().toInt();
    
    m_messageEdit->setText(message);
    m_ringsSpinBox->setValue(rings);
    m_sectorsSpinBox->setValue(sectors);
    
    file.close();
}

void MainWindow::saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Enregistrer la configuration"), "",
        tr("Fichiers de parachute (*.para);;Tous les fichiers (*)"));
        
    if (fileName.isEmpty())
        return;
    
    if (!fileName.endsWith(".para"))
        fileName += ".para";
        
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Erreur d'écriture"),
                             tr("Impossible d'enregistrer le fichier %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out << m_message->getText() << "\n";
    out << m_message->getRings() << "\n";
    out << m_message->getSectors() << "\n";
    
    file.close();
}