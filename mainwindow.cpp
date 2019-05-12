#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QRegularExpression>

#define INTERVAL 2000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mButtons = new QPushButton*[ROWS*COLS];
    mStrings = new QString[ROWS*COLS];
    selected = nullptr;
    isSecondItemSelected = false;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutOccured()));
    resetGame();
}

void MainWindow::buttonPressed() {
    QPushButton *button = (QPushButton *)sender();
    QRegularExpression reg("\\d+");
    QRegularExpressionMatch match = reg.match(button->objectName());
    int index = match.captured(0).toInt()-1;

    if(index == selectedIndex)
        return;

    // if there is no selected item
    if(selected == nullptr){   // objects numbered 1 to 24
        button->setText(mStrings[index]);
        selectedIndex = index;
        selected = mStrings[index];
        return;
    }

    // if there is a selected item and it matches the one selected earlier
    if(index != selectedIndex && QString::compare(selected, mStrings[index]) == 0) {
        button->setText(mStrings[selectedIndex]);
        foundIndices.insert(index);
        foundIndices.insert(selectedIndex);
        selected = nullptr;
        selectedIndex = -1;
        isSecondItemSelected = false;
        return;
    }

    // if there are two selected items but not more
    // this makes sure not more than 2 items gets selected
    if(!isSecondItemSelected){
        button->setText(mStrings[index]);
        isSecondItemSelected = true;
        timer->start(INTERVAL);
    }
}

void MainWindow::timeoutOccured() {
    isSecondItemSelected = false;
    selected = nullptr;
    selectedIndex = -1;
    hideButtonTexts();
    timer->stop();
}

void MainWindow::findButtons() {
    // get a handle for each button
    for(int i = 0; i < ROWS*COLS; i++){
        QString buttonName = "pushButton_" + QString::number(i+1);
        mButtons[i] = MainWindow::findChild<QPushButton*>(buttonName);
        connect(mButtons[i], SIGNAL(released()), this, SLOT(buttonPressed()));
    }
}

void MainWindow::resetGame() {
    findButtons();

    setRandomButtonTexts();

    selected = nullptr;
    selectedIndex = -1;
    isSecondItemSelected = false;
    foundIndices.clear();
    timer->start(INTERVAL);
}

void MainWindow::setRandomButtonTexts() {
    QSet<char> usedChars;
    QSet<int> usedIndices;
    // reset the buttons with random chars
    for(int i = 0; i < ROWS*COLS*0.5; i++) {
        // get a unique char
        char c = 'a';
        do {
            c = static_cast<char>(QRandomGenerator::global()->bounded('A', 'Z'+1));
        }while(usedChars.contains(c));
        usedChars.insert(c);
        mButtons[i]->setText(QString(c));
        mStrings[i] = QString(c);

        // get a unique pair
        int pairIndex = 0;
        do {
            pairIndex = static_cast<int>(QRandomGenerator::global()->bounded(ROWS*COLS/2, ROWS*COLS));
        }while(usedIndices.contains(pairIndex));
        usedIndices.insert(pairIndex);
        mButtons[pairIndex]->setText(QString(c));
        mStrings[pairIndex] = QString(c);
    }
}

void MainWindow::hideButtonTexts() {
    assert(mButtons != nullptr);
    for(int i = 0; i < ROWS*COLS; i++) {
        if(!foundIndices.contains(i))
            mButtons[i]->setText("");
    }
}

void MainWindow::revealButtonTexts() {
    assert(mButtons != nullptr);
    for(int i = 0; i < ROWS*COLS; i++) {
        mButtons[i]->setText(mStrings[i]);
    }
}

void MainWindow::on_Reset_released()
{
    resetGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}


