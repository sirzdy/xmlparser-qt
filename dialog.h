#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QStringListModel>
#include <QSet>
#include <QtXml>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
private:
    void convertXML(QString oldName,QString newName);
    void traverseNode(QDomElement e);
private slots:

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void on_btnConvert_clicked();

    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_btnClear_clicked();

    void on_btnPath_clicked();

    void on_radioOldRate_1_0_clicked();

    void on_radioOldRate_1_5_clicked();

    void on_radioOldRate_2_0_clicked();

    void on_radioNewRate_1_0_clicked();

    void on_radioNewRate_1_5_clicked();

    void on_radioNewRate_2_0_clicked();

private:
    Ui::Dialog *ui;
    QStringListModel *m_mFiles;
    QSet<QString> m_sFiles;
    QString m_sDefaultPath;
    QButtonGroup *m_gOldSize;
    QButtonGroup *m_gNewSize;
    int m_iOldMode;
    int m_iNewMode;


public:
    QStringList listAttrs;

};

#endif // DIALOG_H
