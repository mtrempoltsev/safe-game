#pragma once

#include <QDialog>
#include <QMap>

class QTableWidget;

namespace safe
{
    class HallOfFameDialog
        : public QDialog
    {
        Q_OBJECT
    public:
        explicit HallOfFameDialog(int newTime, QWidget* parent = nullptr);
        ~HallOfFameDialog();

    private:
        void loadData();
        void saveData();
        void fillTable();
        void setDefaultData();
        void checkNewRecord(int time);

    private:
        QTableWidget* table_;

        static constexpr int MaxNames = 10;
        QMultiMap<int, QString> nameByTime_;
    };
}
