#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPushButton>
#include <QRandomGenerator>
#include <QSet>
#include <QTimer>

#define ROWS 4
#define COLS 6

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void resetGame();

private:
    void findButtons();
    void setRandomButtonTexts();
    void hideButtonTexts();
    void revealButtonTexts();

private slots:
    void on_Reset_released();
    void timeoutOccured();
    void buttonPressed();

private:
    Ui::MainWindow *ui;

private:
    QPushButton **mButtons;
    QString *mStrings;
    QSet<int> foundIndices;
    QTimer *timer;
    QString selected;
    bool isSecondItemSelected;
    int selectedIndex;
};

#endif // MAINWINDOW_H
