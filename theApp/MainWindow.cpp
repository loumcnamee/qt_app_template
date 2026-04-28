#include "MainWindow.h"

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    
    setupUi(this);
    
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(900, 600);
        
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        // geometry set by relayout()
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        
        freezeButton = new QPushButton(verticalLayoutWidget);
        freezeButton->setObjectName(QString::fromUtf8("pushButton"));

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(280, 180, 158, 23));
        
        pushButton_3 = new QPushButton(verticalLayoutWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(320, 80, 81, 81));


        pushButton_5 = new QPushButton(verticalLayoutWidget);
        
        
        pushButton_6 = new QPushButton(verticalLayoutWidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        m_button = new QPushButton("Store Content", verticalLayoutWidget);
        verticalLayout->addWidget(m_button);
        //setCentralWidget(m_button);
        connect(m_button, &QPushButton::clicked, this, &MainWindow::storeContent);

        // pushButton_5 = Start, pushButton_3 = Stop
        pushButton_5->setText(QString::fromUtf8("Start"));
        pushButton_3->setText(QString::fromUtf8("Stop"));
        connect(pushButton_5, &QPushButton::clicked, this, &MainWindow::onStartClicked);
        connect(pushButton_3, &QPushButton::clicked, this, &MainWindow::onStopClicked);

        // pushButton = Freeze (zero velocities, keep ticking)
        freezeButton->setText(QString::fromUtf8("Freeze"));
        connect(freezeButton, &QPushButton::clicked, this, &MainWindow::onFreezeClicked);

        verticalLayout->addWidget(pushButton_5);

        

        verticalLayout->addWidget(pushButton_3);
    

        verticalLayout->addWidget(freezeButton);

        

        verticalLayout->addWidget(pushButton_6);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        verticalLayout->addItem(verticalSpacer);

        
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy);
        
        
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy1);
        pushButton_2->setFlat(false);
        
        MainWindow->setCentralWidget(centralwidget);

        // ── Animation mode selector (radio buttons in sidebar) ──────────────
        m_radioLines = new QRadioButton(QString::fromUtf8("Bouncing Lines"), verticalLayoutWidget);
        m_radioBalls = new QRadioButton(QString::fromUtf8("Bouncing Balls"), verticalLayoutWidget);
        m_radioLines->setChecked(true);

        m_modeGroup = new QButtonGroup(this);
        m_modeGroup->addButton(m_radioLines, 0);
        m_modeGroup->addButton(m_radioBalls, 1);
        verticalLayout->addWidget(m_radioLines);
        verticalLayout->addWidget(m_radioBalls);
        connect(m_modeGroup, &QButtonGroup::idClicked,
                this, &MainWindow::onModeChanged);

        // ── Count spinboxes (one per mode, shown/hidden with mode) ───────────
        m_linesCountLabel = new QLabel(QString::fromUtf8("Lines:"), verticalLayoutWidget);
        m_linesCountSpin  = new QSpinBox(verticalLayoutWidget);
        m_linesCountSpin->setRange(1, 50);
        m_linesCountSpin->setValue(aLib::BouncingLinesModel::LINE_COUNT);
        verticalLayout->addWidget(m_linesCountLabel);
        verticalLayout->addWidget(m_linesCountSpin);
        connect(m_linesCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &MainWindow::onLinesCountChanged);

        m_ballsCountLabel = new QLabel(QString::fromUtf8("Balls:"), verticalLayoutWidget);
        m_ballsCountSpin  = new QSpinBox(verticalLayoutWidget);
        m_ballsCountSpin->setRange(1, 50);
        m_ballsCountSpin->setValue(aLib::BouncingBallsModel::BALL_COUNT);
        verticalLayout->addWidget(m_ballsCountLabel);
        verticalLayout->addWidget(m_ballsCountSpin);
        connect(m_ballsCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &MainWindow::onBallsCountChanged);

        // Initially show lines controls, hide balls controls
        m_ballsCountLabel->setVisible(false);
        m_ballsCountSpin->setVisible(false);

        // ── Stacked animation canvas ──────────────────────────────────────────
        m_animStack = new QStackedWidget(centralwidget);

        m_bouncingLines = new BouncingLinesWidget(m_animStack);
        m_bouncingBalls = new BouncingBallsWidget(m_animStack);
        m_animStack->addWidget(m_bouncingLines);  // index 0
        m_animStack->addWidget(m_bouncingBalls);  // index 1
        m_animStack->setCurrentIndex(0);

        // ── Per-ball kinetic energy table (right panel, full height) ────────
        m_keTable = new QTableWidget(0, 2, centralwidget);
        m_keTable->setHorizontalHeaderLabels({QString::fromUtf8("Ball"),
                                              QString::fromUtf8("KE")});
        m_keTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        m_keTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        m_keTable->verticalHeader()->setVisible(false);
        m_keTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_keTable->setSelectionMode(QAbstractItemView::NoSelection);
        m_keTable->setVisible(false);  // shown only in Bouncing Balls mode

        connect(m_bouncingBalls, &BouncingBallsWidget::kineticEnergiesUpdated,
                this, &MainWindow::updateKETable);

        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
        relayout();
    } // setupUi

void MainWindow::relayout()
{
    const int margin   = 10;
    const int sidebarW = 160;
    const int keTableW = 160;

    const int cw = centralwidget->width();
    const int ch = centralwidget->height();

    // Sidebar: fixed width, full available height
    verticalLayoutWidget->setGeometry(margin, margin, sidebarW, ch - 2 * margin);

    // Animation area: largest square that fits after sidebar (and KE table if visible)
    const int rightReserve = m_keTable->isVisible() ? (keTableW + margin) : 0;
    const int availW = cw - sidebarW - 3 * margin - rightReserve;
    const int availH = ch - 2 * margin;
    const int sq = qMin(availW, availH);

    const int animX = sidebarW + 2 * margin;
    m_animStack->setGeometry(animX, margin, sq, sq);

    // KE table: fills remaining width to the right of the animation area
    if (m_keTable->isVisible())
        m_keTable->setGeometry(animX + sq + margin, margin,
                               cw - animX - sq - 2 * margin, ch - 2 * margin);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    relayout();
}

void MainWindow::storeContent()
{
    //qDebug() << "... store content";
    QString message("Hello World!");
    QFile file(QDir::home().absoluteFilePath("out.txt"));
    if(!file.open(QIODevice::WriteOnly)) {
        //qWarning() << "Can not open file with write access";
        return;
    }
    QTextStream stream(&file);
    stream << message;
}

void MainWindow::onModeChanged(int id)
{
    m_animStack->setCurrentIndex(id);
    // Show the KE table only when Bouncing Balls (id=1) is active
    m_keTable->setVisible(id == 1);
    // Show the matching count control
    m_linesCountLabel->setVisible(id == 0);
    m_linesCountSpin->setVisible(id == 0);
    m_ballsCountLabel->setVisible(id == 1);
    m_ballsCountSpin->setVisible(id == 1);
    relayout();
}

void MainWindow::updateKETable(QVector<double> energies)
{
    const int n = energies.size();
    m_keTable->setRowCount(n);

    for (int i = 0; i < n; ++i) {
        // Ball number column
        auto* numItem = new QTableWidgetItem(QString::number(i + 1));
        numItem->setTextAlignment(Qt::AlignCenter);
        m_keTable->setItem(i, 0, numItem);

        // KE value column
        auto* keItem = new QTableWidgetItem(
            QString::number(energies[i], 'f', 1));
        keItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_keTable->setItem(i, 1, keItem);
    }
}

void MainWindow::onStartClicked()
{
    m_bouncingLines->start();
    m_bouncingBalls->start();
}

void MainWindow::onStopClicked()
{
    m_bouncingLines->stop();
    m_bouncingBalls->stop();
}

void MainWindow::onFreezeClicked()
{
    m_bouncingLines->freeze();
    m_bouncingBalls->freeze();
}

void MainWindow::onLinesCountChanged(int value)
{
    m_bouncingLines->setCount(value);
}

void MainWindow::onBallsCountChanged(int value)
{
    m_bouncingBalls->setCount(value);
}

 void MainWindow::retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
                
        
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        freezeButton->setText(QCoreApplication::translate("MainWindow", "Freeze", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "PushButton 6", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "PushButton 4", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "PushButton 2", nullptr));
    } // retranslateUi
