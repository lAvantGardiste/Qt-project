#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QMenuBar>
#include <QAction>
#include <QGroupBox>
#include <QApplication>
#include <QStyleFactory>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QDataStream>
#include "view/parachuteview.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_message(new Message)
{
    setupUI();
    createMenus();
    setWindowTitle(tr("Encodage de message dans un parachute"));

    // Style avec menus visibles
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: white;
        }
        QMenuBar {
            background-color: white;
            color: black;
            border-bottom: 1px solid #999;
        }
        QMenuBar::item {
            background-color: white;
            color: black;
            padding: 4px 10px;
        }
        QMenuBar::item:selected {
            background-color: #e0e0e0;
        }
        QMenu {
            background-color: white;
            color: black;
            border: 1px solid #999;
        }
        QMenu::item {
            padding: 4px 20px;
        }
        QMenu::item:selected {
            background-color: #e0e0e0;
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

void MainWindow::setupUI()
{
    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    auto layout = new QVBoxLayout(centralWidget);
    
    // Message group
    auto messageGroup = new QGroupBox(tr("Message"), centralWidget);
    auto messageLayout = new QVBoxLayout(messageGroup);
    m_messageEdit = new QLineEdit(messageGroup);
    m_messageEdit->setPlaceholderText(tr("Enter your message here"));
    messageLayout->addWidget(m_messageEdit);
    
    // Parameters group
    auto controlsGroup = new QGroupBox(tr("Parameters"), centralWidget);
    auto controlsLayout = new QGridLayout(controlsGroup);
    
    // Rings controls
    auto ringsLabel = new QLabel(tr("Number of tracks:"), controlsGroup);
    m_ringsSpinBox = new QSpinBox(controlsGroup);
    m_ringsSpinBox->setRange(1, 10);
    m_ringsSpinBox->setValue(5);
    m_ringsSpinBox->setButtonSymbols(QSpinBox::PlusMinus);
    m_ringsSpinBox->setAlignment(Qt::AlignCenter);
    m_ringsSpinBox->setFixedWidth(100);
    m_ringsSpinBox->setStyle(QStyleFactory::create("Fusion"));
    m_ringsSlider = new QSlider(Qt::Horizontal, controlsGroup);
    m_ringsSlider->setRange(1, 10);
    m_ringsSlider->setValue(5);
    
    // Sectors controls
    auto sectorsLabel = new QLabel(tr("Number of sectors:"), controlsGroup);
    m_sectorsSpinBox = new QSpinBox(controlsGroup);
    m_sectorsSpinBox->setRange(4, 32);
    m_sectorsSpinBox->setValue(21);
    m_sectorsSpinBox->setButtonSymbols(QSpinBox::PlusMinus);
    m_sectorsSpinBox->setAlignment(Qt::AlignCenter);
    m_sectorsSpinBox->setFixedWidth(100);
    m_sectorsSpinBox->setStyle(QStyleFactory::create("Fusion"));
    m_sectorsSlider = new QSlider(Qt::Horizontal, controlsGroup);
    m_sectorsSlider->setRange(4, 32);
    m_sectorsSlider->setValue(21);
    
    controlsLayout->addWidget(ringsLabel, 0, 0);
    controlsLayout->addWidget(m_ringsSpinBox, 0, 1);
    controlsLayout->addWidget(m_ringsSlider, 1, 0, 1, 2);
    controlsLayout->addWidget(sectorsLabel, 2, 0);
    controlsLayout->addWidget(m_sectorsSpinBox, 2, 1);
    controlsLayout->addWidget(m_sectorsSlider, 3, 0, 1, 2);
    
    // Display group
    auto viewGroup = new QGroupBox(tr("Display"), centralWidget);
    auto viewLayout = new QVBoxLayout(viewGroup);
    
    // Binary view checkbox
    auto binaryViewCheck = new QCheckBox(tr("Binary View"), viewGroup);
    viewLayout->addWidget(binaryViewCheck);
    
    // Parachute view
    m_parachuteView = new ParachuteView(m_message, viewGroup);
    viewLayout->addWidget(m_parachuteView);
    
    // Add all groups to main layout
    layout->addWidget(messageGroup);
    layout->addWidget(controlsGroup);
    layout->addWidget(viewGroup);
    
    // Connect signals
    connect(m_messageEdit, &QLineEdit::textChanged, this, &MainWindow::updateMessage);
    connect(m_ringsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateRings);
    connect(m_ringsSlider, &QSlider::valueChanged, m_ringsSpinBox, &QSpinBox::setValue);
    connect(m_ringsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_ringsSlider, &QSlider::setValue);
    connect(m_sectorsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateSectors);
    connect(m_sectorsSlider, &QSlider::valueChanged, m_sectorsSpinBox, &QSpinBox::setValue);
    connect(m_sectorsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_sectorsSlider, &QSlider::setValue);
    connect(binaryViewCheck, &QCheckBox::toggled, m_parachuteView, &ParachuteView::setBinaryViewMode);
    
    // Initial update
    updateMessage();
}

void MainWindow::createMenus() {
    // File Menu
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

    // Edit Menu
    auto editMenu = menuBar()->addMenu(tr("&Edition"));
    
    auto copyAction = new QAction(tr("&Copier"), this);
    copyAction->setShortcuts(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, m_messageEdit, &QLineEdit::copy);
    editMenu->addAction(copyAction);
    
    auto pasteAction = new QAction(tr("Co&ller"), this);
    pasteAction->setShortcuts(QKeySequence::Paste);
    connect(pasteAction, &QAction::triggered, m_messageEdit, &QLineEdit::paste);
    editMenu->addAction(pasteAction);

    // View Menu
    auto viewMenu = menuBar()->addMenu(tr("&Affichage"));
    
    auto binaryViewAction = new QAction(tr("Vue &binaire"), this);
    binaryViewAction->setCheckable(true);
    connect(binaryViewAction, &QAction::toggled, this, &MainWindow::toggleBinaryView);
    viewMenu->addAction(binaryViewAction);

    // Language Menu
    auto langMenu = menuBar()->addMenu(tr("&Langue"));
    
    m_frenchAction = new QAction(tr("&Français"), this);
    m_frenchAction->setCheckable(true);
    m_frenchAction->setChecked(true);
    langMenu->addAction(m_frenchAction);
    
    m_englishAction = new QAction(tr("&English"), this);
    m_englishAction->setCheckable(true);
    langMenu->addAction(m_englishAction);
    
    // Make language actions exclusive
    auto langGroup = new QActionGroup(this);
    langGroup->addAction(m_frenchAction);
    langGroup->addAction(m_englishAction);
    langGroup->setExclusive(true);
    
    connect(langGroup, &QActionGroup::triggered, this, &MainWindow::changeLanguage);

    // Help Menu
    auto helpMenu = menuBar()->addMenu(tr("&Aide"));
    
    auto aboutAction = new QAction(tr("À &propos"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    helpMenu->addAction(aboutAction);
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

void MainWindow::changeLanguage(QAction* action) {
    QString language = action->text().contains("English") ? "en" : "fr";
    
    // Remove old translator if it exists
    qApp->removeTranslator(&m_translator);
    
    // Load new translation
    if (language == "en") {
        if (m_translator.load(":/translations/parachute_en.qm")) {
            qApp->installTranslator(&m_translator);
        }
    }
    
    // Update window title
    setWindowTitle(language == "en" ? tr("Message Encoding in a Parachute") : tr("Encodage de message dans un parachute"));

    // Update all group boxes
    auto messageGroup = centralWidget()->findChild<QGroupBox*>();
    if (messageGroup) messageGroup->setTitle(tr("Message"));

    auto controlsGroup = centralWidget()->findChildren<QGroupBox*>().at(1);
    if (controlsGroup) controlsGroup->setTitle(language == "en" ? tr("Parameters") : tr("Paramètres"));

    auto viewGroup = centralWidget()->findChildren<QGroupBox*>().at(2);
    if (viewGroup) viewGroup->setTitle(language == "en" ? tr("Display") : tr("Affichage"));

    // Update labels
    auto labels = centralWidget()->findChildren<QLabel*>();
    for (auto label : labels) {
        if (label->text().contains("tracks") || label->text().contains("pistes"))
            label->setText(language == "en" ? tr("Number of tracks:") : tr("Nombre de pistes:"));
        else if (label->text().contains("sectors") || label->text().contains("secteurs"))
            label->setText(language == "en" ? tr("Number of sectors:") : tr("Nombre de secteurs:"));
    }

    // Update checkbox
    auto binaryViewCheck = centralWidget()->findChild<QCheckBox*>();
    if (binaryViewCheck) binaryViewCheck->setText(language == "en" ? tr("Binary View") : tr("Vue binaire"));

    // Update line edit placeholder
    if (m_messageEdit) {
        m_messageEdit->setPlaceholderText(language == "en" ? tr("Enter your message here") : tr("Entrez votre message ici"));
    }

    // Store the current checked states before clearing menus
    bool wasEnglish = language == "en";

    // Update menus
    menuBar()->clear();
    createMenus();

    // Restore the correct checked state
    if (wasEnglish) {
        m_englishAction->setChecked(true);
    } else {
        m_frenchAction->setChecked(true);
    }

    // Force update of the parachute view
    if (m_parachuteView) m_parachuteView->update();
}

void MainWindow::showAbout() {
    QMessageBox::about(this, tr("À propos"),
        tr("Encodage de message dans un parachute\n\n"
           "Version 1.0\n"
           "© 2024 ENSI Caen\n\n"
           "Programme permettant d'encoder des messages dans un parachute "
           "en utilisant une représentation binaire."));
}

void MainWindow::loadTranslations() {
    // Default to French - no translation file needed as it's the default
    // English translations will be loaded when user switches language
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Ouvrir un fichier"), "",
        tr("Fichiers parachute (*.prc);;Tous les fichiers (*)"));
        
    if (fileName.isEmpty())
        return;
        
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Erreur"),
            tr("Impossible d'ouvrir le fichier %1:\n%2.")
            .arg(QDir::toNativeSeparators(fileName),
                 file.errorString()));
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);

    QString message;
    int rings, sectors;
    bool binaryView;
    
    in >> message >> rings >> sectors >> binaryView;
    
    if (in.status() != QDataStream::Ok) {
        QMessageBox::warning(this, tr("Erreur"),
            tr("Erreur lors de la lecture du fichier %1")
            .arg(QDir::toNativeSeparators(fileName)));
        return;
    }
    
    m_messageEdit->setText(message);
    m_ringsSpinBox->setValue(rings);
    m_sectorsSpinBox->setValue(sectors);
    // Find and toggle the binary view action if needed
    auto viewMenu = menuBar()->actions().at(2)->menu();
    if (viewMenu) {
        auto binaryViewAction = viewMenu->actions().first();
        if (binaryViewAction) {
            binaryViewAction->setChecked(binaryView);
        }
    }
}

void MainWindow::saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Enregistrer le fichier"), "",
        tr("Fichiers parachute (*.prc);;Tous les fichiers (*)"));
        
    if (fileName.isEmpty())
        return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Erreur"),
            tr("Impossible d'enregistrer le fichier %1:\n%2.")
            .arg(QDir::toNativeSeparators(fileName),
                 file.errorString()));
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0);
    
    // Save the current state
    out << m_messageEdit->text()
        << m_ringsSpinBox->value()
        << m_sectorsSpinBox->value()
        << m_parachuteView->isBinaryViewMode();
        
    if (out.status() != QDataStream::Ok) {
        QMessageBox::warning(this, tr("Erreur"),
            tr("Erreur lors de l'écriture du fichier %1")
            .arg(QDir::toNativeSeparators(fileName)));
    }
}