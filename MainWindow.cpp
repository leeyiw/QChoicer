#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    baseLayout = new QHBoxLayout();

    /* left side: question list */
    questionListWidget = new QListWidget(this);
    baseLayout->addWidget(questionListWidget);

    /* right side: vertical layout */
    rightLayout = new QVBoxLayout();
    baseLayout->addLayout(rightLayout);

    /* right top side: question area with vertical layout */
    questionLayout = new QVBoxLayout();
    questionLayout->setAlignment(Qt::AlignTop);
    questionGroupBox = new QGroupBox(this);
    questionGroupBox->setLayout(questionLayout);
    questionLabel = new QLabel(this);
    questionLayout->addWidget(questionLabel);
    connect(&optionBtnGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(optionBtnClicked(int)));
    rightLayout->addWidget(questionGroupBox);

    /* right bottom side: buttons with horizontal layout */
    operationLayout = new QHBoxLayout();
    prevBtn = new QPushButton("上一题", this);
    connect(prevBtn, SIGNAL(clicked()), this, SLOT(prevBtnClicked()));
    nextBtn = new QPushButton("下一题", this);
    connect(nextBtn, SIGNAL(clicked()), this, SLOT(nextBtnClicked()));
    submitBtn = new QPushButton("提交", this);
    connect(submitBtn, SIGNAL(clicked()), this, SLOT(submitBtnClicked()));
    resetBtn = new QPushButton("重新答题", this);
    connect(resetBtn, SIGNAL(clicked()), this, SLOT(resetBtnClicked()));
    operationLayout->addWidget(prevBtn);
    operationLayout->addWidget(nextBtn);
    operationLayout->addWidget(submitBtn);
    operationLayout->addWidget(resetBtn);
    rightLayout->addLayout(operationLayout);

    QWidget *widget = new QWidget(this);
    widget->setLayout(baseLayout);
    setCentralWidget(widget);

    loadQuestions();
}

MainWindow::~MainWindow()
{

}

void
MainWindow::questionChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    int i = current->data(Qt::UserRole).toInt();
    Question &q = questionList[i];
    questionGroupBox->setTitle(QString("第%1题").arg(i + 1));
    questionLabel->setText(q.question);
    for (int i = 0; ; i++) {
        QRadioButton *btn = (QRadioButton *)optionBtnGroup.button(i);
        if (btn == NULL) {
            break;
        }
        questionLayout->removeWidget(btn);
        optionBtnGroup.removeButton(btn);
        delete btn;
    }
    for (int i = 0; i < q.options.size(); i++) {
        QRadioButton *btn = new QRadioButton(q.options[i]);
        if (q.selection != -1 && q.selection == i) {
            btn->setChecked(true);
        }
        questionLayout->addWidget(btn);
        optionBtnGroup.addButton(btn, i);
    }
    int row = questionListWidget->row(current);
    prevBtn->setEnabled(row != 0);
    nextBtn->setEnabled(row != questionList.size() - 1);
}

void
MainWindow::optionBtnClicked(int id)
{
    Question &q = questionList[questionListWidget->currentRow()];
    q.selection = id;
}

void
MainWindow::prevBtnClicked()
{
    questionListWidget->setCurrentRow(questionListWidget->currentRow() - 1);
}

void
MainWindow::nextBtnClicked()
{
    questionListWidget->setCurrentRow(questionListWidget->currentRow() + 1);
}

void
MainWindow::submitBtnClicked()
{
    int n = 0;
    int first_unanswered = -1;
    for (int i = 0; i < questionList.size(); i++) {
        Question &q = questionList[i];
        if (q.selection == -1) {
            n++;
            if (first_unanswered == -1) {
                first_unanswered = i;
            }
        }
    }
    if (n != 0) {
        QMessageBox::StandardButton btn;
        btn = QMessageBox::warning(this, "提交",
                                   QString("您还有%1道题未作答，确认提交？").arg(n),
                                   QMessageBox::Ok|QMessageBox::Cancel);
        if (btn != QMessageBox::Ok) {
            return;
        }
    }
}

void
MainWindow::resetBtnClicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::warning(this, "重新答题",
                               "确认重新答题？之前的答题记录将被会清除！",
                               QMessageBox::Ok|QMessageBox::Cancel);
    if (btn != QMessageBox::Ok) {
        return;
    }
    loadQuestions();
}

void
MainWindow::loadQuestions()
{
    /* clear questions first */
    questionList.clear();
    disconnect(questionListWidget,
               SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
               0, 0);
    while (true) {
        QListWidgetItem *item = questionListWidget->takeItem(0);
        if (item == NULL) {
            break;
        }
        delete item;
    }

    /* load questions from ini file */
    QSettings settings("questions.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QStringList groups = settings.childGroups();
    for (int i = 0; i < groups.size(); i++) {
        settings.beginGroup(groups.at(i));
        Question q(settings);
        questionList.append(q);
        settings.endGroup();
        qDebug() << q.question;
        qDebug() << q.options;
        qDebug() << q.answer;
    }
    /* randomize question list */
    qsrand(QTime::currentTime().msec());
    for (int i = 0; i < questionList.size() / 2; i++) {
        int a = qrand() % questionList.size();
        int b = qrand() % questionList.size();
        questionList.swap(a, b);
    }
    for (int i = 0; i < questionList.size(); i++) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(QString("第%1题").arg(i + 1));
        item->setData(Qt::UserRole, i);
        questionListWidget->addItem(item);
    }
    connect(questionListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(questionChanged(QListWidgetItem*,QListWidgetItem*)));
    questionListWidget->setCurrentRow(0);
}
