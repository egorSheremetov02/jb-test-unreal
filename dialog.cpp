#include <QEventLoop>
#include <QTimer>
#include "dialog.h"
#include "ui_dialog.h"
#include "lazy_list_model.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog) {

    model = new LazyWordList(this, "/home/egor/Qt/jb-test-unreal/resources/dictionary.txt");
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &Dialog::onTimerTimeout);

    ui->setupUi(this);
    ui->listView->setModel(model);
    ui->listView->setBatchSize(1000);
    ui->listView->setLayoutMode(QListView::LayoutMode::Batched);

    cur_input = "";
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::onTimerTimeout() {
    model->set_substring(cur_input);
}


void Dialog::on_lineEdit_textEdited(const QString &cur_text) {
    cur_input = cur_text;
    timer->start(300);
}
