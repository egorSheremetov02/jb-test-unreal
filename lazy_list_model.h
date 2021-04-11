#ifndef LAZY_LIST_MODEL_H
#define LAZY_LIST_MODEL_H
#include <QAbstractListModel>
#include <QList>
#include <QStringList>
#include <vector>

class LazyWordList : public QAbstractListModel {
    Q_OBJECT

public:
    LazyWordList(QObject *parent, QString const &file_path);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

signals:
    void numberPopulated(int number);

public slots:
    void set_substring(const QString &s);

protected:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

private:
//    QStringList words_list;
    QStringList filtered_list;
    QStringList all_words;
    std::string substring;
    int words_count;
//    std::vector<int> zeta_function;
//    std::vector<std::vector<int>> pos;
//    std::vector<int> shift;
};

#endif // LAZY_LIST_MODEL_H
