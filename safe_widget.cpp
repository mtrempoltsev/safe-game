#include "safe_widget.h"

#include <QDateTime>
#include <QGridLayout>
#include <QTimer>

#include "switch_widget.h"

safe::SafeWidget::SafeWidget(int size, QWidget* parent)
    : QFrame(parent)
    , image_(":/images/switch.png")
    , size_(size)
    , totalSwitches_(size * size)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    switches_ = new QGridLayout();

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            const Orientation orient = static_cast<Orientation>(qrand() % OrientationCount);
            if (orient == Horizontal)
            {
                ++horizontalSwitches_;
            }

            auto switchWidget = new SwitchWidget(image_, j, i, orient, this);
            connect(switchWidget, &SwitchWidget::orientationChanged, this, &SafeWidget::onOrientationChanged);
            connect(switchWidget, &SwitchWidget::clicked, this, &SafeWidget::onSwitchClicked);
            switches_->addWidget(switchWidget, j, i);
        }
    }

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    setLayout(switches_);
}

void safe::SafeWidget::onOrientationChanged(Orientation orient)
{
    --animatedSwitches_;
    if (orient == Horizontal)
    {
        ++horizontalSwitches_;
    }
    checkWinCondition();
}

void safe::SafeWidget::checkWinCondition()
{
    if (horizontalSwitches_ == totalSwitches_)
    {
        emit playerWon();
    }
}

void safe::SafeWidget::onSwitchClicked(int row, int column)
{
    if (animatedSwitches_ > 0)
    {
        return;
    }

    changeSwitchOrientation(row, column);

    lines_ = { row, column, column - 1, column + 1, row - 1, row + 1 };

    QTimer::singleShot(timeoutToAnimateNextMsec, this, &SafeWidget::onTryToSwitchNext);
}

void safe::SafeWidget::onTryToSwitchNext()
{
    int fails = 0;

    auto tryToSwitch = [&fails, this](int row, int column)
    {
        if (isValidCoord(row, column))
        {
            changeSwitchOrientation(row, column);
        }
        else
        {
            ++fails;
        }
    };

    tryToSwitch(lines_.row, lines_.up);
    tryToSwitch(lines_.row, lines_.down);
    tryToSwitch(lines_.left, lines_.column);
    tryToSwitch(lines_.right, lines_.column);

    if (fails < 4)
    {
        --lines_.up;
        ++lines_.down;
        --lines_.left;
        ++lines_.right;

        QTimer::singleShot(timeoutToAnimateNextMsec, this, &SafeWidget::onTryToSwitchNext);
    }
}

void safe::SafeWidget::changeSwitchOrientation(int row, int column)
{
    ++animatedSwitches_;

    auto item = switches_->itemAtPosition(row, column);
    auto switchWidget = static_cast<SwitchWidget*>(item->widget());

    if (switchWidget->orientation() == Horizontal)
    {
        --horizontalSwitches_;
    }

    switchWidget->changeOrientation();
}

bool safe::SafeWidget::isValidCoord(int row, int column) const
{
    return row >= 0 && row < size_ && column >= 0 && column < size_;
}
