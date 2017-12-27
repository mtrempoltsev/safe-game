#include "main_window.h"

#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>

#include "hall_of_fame_dialog.h"
#include "safe_widget.h"
#include "utils.h"

safe::MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Safe Game"));

    updateTime_ = new QTimer(this);
    updateTime_->setInterval(1000); // one second
    connect(updateTime_, &QTimer::timeout, this, &MainWindow::onUpdateTime);

    time_ = new QLabel();

    auto info = new QHBoxLayout();
    info->addStretch();
    info->addWidget(time_);
    info->addStretch();

    placeForSafe_ = new QHBoxLayout();

    size_ = new QComboBox();
    for (auto i = MinSize; i <= MaxSize; ++i)
    {
        size_->addItem(QString::number(i), i);
    }

    auto restart = new QPushButton(tr("Restart"));
    connect(restart, &QPushButton::clicked, this, &MainWindow::onStartNewGame);

    auto controls = new QHBoxLayout();
    controls->addStretch();
    controls->addWidget(size_);
    controls->addWidget(restart);
    controls->addStretch();

    auto layout = new QVBoxLayout();
    layout->addLayout(info);
    layout->addLayout(placeForSafe_);
    layout->addStretch();
    layout->addLayout(controls);

    auto frame = new QFrame();
    frame->setLayout(layout);

    setCentralWidget(frame);

    onStartNewGame();
}

safe::MainWindow::~MainWindow()
{
}

void safe::MainWindow::onPlayerWon()
{
    updateTime_->stop();
    HallOfFameDialog dialog(secondsFromStart_);
    dialog.exec();
}

void safe::MainWindow::onUpdateTime()
{
    ++secondsFromStart_;
    showTime();
}

void safe::MainWindow::onStartNewGame()
{
    auto old = placeForSafe_->takeAt(0);
    if (old != nullptr && !old->isEmpty())
    {
        delete old->widget();
    }

    secondsFromStart_ = 0;
    showTime();
    updateTime_->start();

    const auto size = size_->currentData().toInt();

    auto safe = new SafeWidget(size);
    connect(safe, &SafeWidget::playerWon, this, &MainWindow::onPlayerWon);

    placeForSafe_->addWidget(safe);

    centralWidget()->adjustSize();
    adjustSize();
}

void safe::MainWindow::showTime()
{
    time_->setText(secondsToString(secondsFromStart_));
}
