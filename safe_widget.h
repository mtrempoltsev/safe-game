#pragma once

#include <QFrame>
#include <QPixmap>

#include "orientation.h"

class QGridLayout;

namespace safe
{
    class SafeWidget
        : public QFrame
    {
        Q_OBJECT
    public:
        explicit SafeWidget(int size, QWidget* parent = nullptr);

    public slots:
        void onOrientationChanged(Orientation orient);
        void onSwitchClicked(int row, int column);

        void onTryToSwitchNext();

    signals:
        void playerWon();

    private:
        void checkWinCondition();
        void changeSwitchOrientation(int row, int column);
        bool isValidCoord(int row, int column) const;

    private:
        QPixmap image_;

        const int size_;
        const int totalSwitches_;
        int animatedSwitches_ = 0;
        int switchOrientations_[OrientationCount] = { 0, 0 };

        QGridLayout* switches_;

        static constexpr int timeoutToAnimateNextMsec = 100;

        struct Lines
        {
            int row;
            int column;

            int up;
            int down;
            int left;
            int right;
        };

        Lines lines_ = {};
    };
}
