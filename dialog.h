#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QTimer>
#include <QStringListModel>
#include "lazy_list_model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog {
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void onTimerTimeout();

private slots:
    void on_lineEdit_textEdited(const QString &cur_text);

private:
    Ui::Dialog *ui;
    LazyWordList *model;
    QTimer *timer;
    QStringList list;
    QString cur_input;
};
#endif // DIALOG_H
