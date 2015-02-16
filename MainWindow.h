#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QVBoxLayout>
#include <QDebug>

#include "Question.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void questionChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void optionBtnClicked(int id);
    void prevBtnClicked();
    void nextBtnClicked();
    void submitBtnClicked();
    void resetBtnClicked();

private:
    void loadQuestions();

    QHBoxLayout *baseLayout;
    QVBoxLayout *rightLayout;
    QVBoxLayout *questionLayout;
    QHBoxLayout *operationLayout;
    QListWidget *questionListWidget;
    QGroupBox *questionGroupBox;
    QLabel *questionLabel;
    QButtonGroup optionBtnGroup;
    QPushButton *prevBtn, *nextBtn, *submitBtn, *resetBtn;

    QList<Question> questionList;
};

#endif // MAINWINDOW_H
