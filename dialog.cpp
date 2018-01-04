#include "dialog.h"
#include "ui_dialog.h"


void Dialog::traverseNode(QDomElement e)
{
    QDomNamedNodeMap attrs = e.attributes();
    QString tag = e.tagName();
    // 遍历属性
    for (int i = 0; i < attrs.count(); i++)
    {
        QDomNode attr =  attrs.item(i);
        if (!attr.isNull() && attr.isAttr()) {
            QString att = attr.nodeName();
            QString val = attr.nodeValue();
            if (listAttrs.contains(att))
            {
                QString  afterVal;
                QStringList list = val.split(",");
                QStringList l;
                for (int i = 0; i < list.size(); i++)
                {
                    QString str = list[i];
                    bool ok;
                    int dec = str.toInt(&ok, 10);
                    QString v;
                    if (ok)
                    {
                        if (dec % m_iOldMode != 0)
                        {

                            ui->textLog->append("-<建议调整为" + QString::number(m_iOldMode) + "的整数倍>" + e.nodeName() + " " + attr.nodeName() + ":" + attr.nodeValue());
                        }
                        int target;
                        if (dec != 0 && dec * m_iNewMode / m_iOldMode == 0)
                        {
                            target = 1;
                            ui->textLog->append("-<最小尺寸置为1>");
                        }
                        else
                        {
                            target = dec * m_iNewMode / m_iOldMode;
                        }
                        l.append(QString::number(target, 10));
                    }
                }
                afterVal = l.join(",");
                e.setAttribute(att, afterVal);
            }
            if (att == "value")
            {
                QHash<QString, QString> hash;
                QString afterValue = "";
                QStringList listVal = val.split("\" ");
                // 拆分字符串到hash
                foreach (QString s, listVal) {
                    QString key = s.left(s.indexOf("="));
                    QString value = s.right(s.size() - s.indexOf("=") - 1).replace("\"", "");
                    hash.insert(key, value);
                }
                // 处理hash 并 拼接字符串
                foreach (QString s, hash.keys()) {
                    if (listAttrs.contains(s))
                    {

                        QString  afterVal;
                        QString val = hash[s];
                        QStringList list = val.split(",");
                        QStringList l;
                        for (int i = 0; i < list.size(); i++)
                        {
                            QString str = list[i];
                            bool ok;
                            int dec = str.toInt(&ok, 10);
                            QString v;
                            if (ok)
                            {
                                if (dec % m_iOldMode != 0)
                                {

                                    ui->textLog->append("-<建议调整为" + QString::number(m_iOldMode) + "的整数倍>" + e.nodeName() + " " + attr.nodeName() + ":" + attr.nodeValue());
                                }
                                int target;
                                if (dec != 0 && dec * m_iNewMode / m_iOldMode == 0)
                                {
                                    target = 1;
                                    ui->textLog->append("-<最小尺寸置为1>");
                                }
                                else
                                {
                                    target = dec * m_iNewMode / m_iOldMode;
                                }
                                l.append(QString::number(target, 10));
                            }
                        }
                        afterVal = l.join(",");

                        hash[s] = afterVal;
                    }

                    afterValue.append(s).append("=\"").append(hash[s]).append("\" ");
                }

                e.setAttribute(att, afterValue);
            }
        }
    }

    // 遍历子节点
    QDomNode child = e.firstChild();
    while (!child.isNull() )
    {
        if(child.isElement())
        {
            QDomElement childele = child.toElement(); // try to convert the node to an element.
            traverseNode(childele);
        }
        else if(child.isComment()||child.isNotation())//注释
        {

        }
        child = child.nextSibling();
    }
}

void Dialog::convertXML(QString oldName, QString newName)
{
    ui->textLog->append("【START】 " + oldName + " → " + newName);
    QString filename = oldName;
    QFile file(filename);
    QDomDocument document;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file";
        ui->textLog->append("打开源文件失败");
        return;
    }
    else
    {
        if (!document.setContent(&file))
        {
            qDebug() << "Failed to open document";
            ui->textLog->append("解析源文件失败");
            return;
        }
        file.close();
    }

    QDomElement root = document.documentElement();
    traverseNode(root);

    QFile newFile(newName);
    if (!newFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file";
        ui->textLog->append("打开目标文件失败");
        return;
    }
    else
    {
        QTextStream stream(&newFile);
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        stream.setCodec(codec);
        QString str = document.toString().replace(QRegExp("version='1.0'",Qt::CaseInsensitive),"version=\"1.0\"").replace(QRegExp("encoding='utf-8'",Qt::CaseInsensitive),"encoding=\"utf-8\"").replace(QRegExp("standalone='yes'",Qt::CaseInsensitive),"standalone=\"yes\"").replace(QRegExp("/>"), " />").replace(QRegExp("\r"), "\r\n");
        stream << str;
        newFile.close();
        qDebug() << "Finished";
    }
    ui->textLog->append("【END】 " + oldName + " → " + newName);
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    listAttrs << "size" << "caption" << "width" << "height" << "maxwidth" << "maxheight" << "minwidth" << "minheight" << "iconsize" << "itemsize" << "dropboxsize" << "thumbsize" << "itemminwidth" << "itemtextpadding" << "sepheight" << "sepwidth" << "inset"  << "padding" << "childpadding" << "textpadding" << "sizebox" << "mininfo" << "maxinfo" << "pos" << "roundcorner" << "borderround" << "bordersize";

    m_mFiles = new QStringListModel(this);
    ui->listFiles->setModel(m_mFiles);
    m_sDefaultPath = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0];
    // 尺寸（1.0x）
    m_gOldSize = new QButtonGroup(this);
    m_gOldSize->addButton(ui->radioOldRate_1_0);
    m_gOldSize->addButton(ui->radioOldRate_1_5);
    m_gOldSize->addButton(ui->radioOldRate_2_0);

    m_gNewSize = new QButtonGroup(this);
    m_gNewSize->addButton(ui->radioNewRate_1_0);
    m_gNewSize->addButton(ui->radioNewRate_1_5);
    m_gNewSize->addButton(ui->radioNewRate_2_0);

    // 设置初始模式 1.0x-1.0x
    m_iOldMode = 2;
    m_iNewMode = 2;
    ui->radioOldRate_1_0->setChecked(true);
    ui->radioNewRate_1_0->setChecked(true);
}

Dialog::~Dialog()
{
    if (m_mFiles) {
        delete m_mFiles;
        m_mFiles = nullptr;
    }
    if (m_gOldSize) {
        delete m_gOldSize;
        m_gOldSize = nullptr;
    }
    if (m_gNewSize) {
        delete m_gNewSize;
        m_gNewSize = nullptr;
    }
    delete ui;
}


void Dialog::on_btnConvert_clicked()
{
    ui->btnConvert->setEnabled(false);
    // 存储路径
    QString strPath;
    strPath = ui->editPath->text();
    if (m_sFiles.size() <= 0)
    {
        QMessageBox msgBox;
        msgBox.setInformativeText("请选择文件");
        msgBox.setText("注意");
        msgBox.exec();
    }
    foreach (QString f, m_sFiles) {
        // 文件名（含扩展名）
        QString fullname = f.right(f.size() - f.lastIndexOf("/") - 1);
        // 文件名（不含扩展名）
        QString name = fullname.left(fullname.lastIndexOf("."));
        // 原扩展名
        QString ext = fullname.right(fullname.size() - fullname.lastIndexOf(".") - 1);
        // 存储路径
        QString path = strPath.isEmpty() ? f.left(f.lastIndexOf("/")) : strPath;
        // 存储名称
        QString filename = path + "/" + (strPath.isEmpty() ? name + "_new." + ext : fullname);
        qDebug() << f << filename << m_iOldMode << m_iNewMode;
        convertXML(f, filename);
    }
    ui->btnConvert->setEnabled(true);
}

void Dialog::on_btnAdd_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "选择一个或多个文件",
                            m_sDefaultPath,
                            "xml (*.xml)");

    foreach (QString f, files) {
        m_sFiles.insert(f);
    }
    m_mFiles->setStringList(m_sFiles.toList());
}

void Dialog::on_btnDelete_clicked()
{
    m_mFiles->removeRows(ui->listFiles->currentIndex().row(), 1);
    m_sFiles = QSet<QString>::fromList(m_mFiles->stringList());
}

void Dialog::on_btnClear_clicked()
{
    m_sFiles.clear();
    m_mFiles->setStringList(m_sFiles.toList());
}

void Dialog::on_btnPath_clicked()
{
    QString strPath = QFileDialog::getExistingDirectory(
                          this,
                          "选择存储路径",
                          ui->editPath->text().isEmpty() ? m_sDefaultPath : ui->editPath->text());
    ui->editPath->setText(strPath);
    ui->editPath->repaint();
}

void Dialog::on_radioOldRate_1_0_clicked()
{
    m_iOldMode = 2;
}

void Dialog::on_radioOldRate_1_5_clicked()
{
    m_iOldMode = 3;
}

void Dialog::on_radioOldRate_2_0_clicked()
{
    m_iOldMode = 4;
}

void Dialog::on_radioNewRate_1_0_clicked()
{
    m_iNewMode = 2;
}

void Dialog::on_radioNewRate_1_5_clicked()
{
    m_iNewMode = 3;
}

void Dialog::on_radioNewRate_2_0_clicked()
{
    m_iNewMode = 4;
}

//当用户拖动文件到窗口部件上时候，就会触发dragEnterEvent事件
void Dialog::dragEnterEvent(QDragEnterEvent *event)
{
    //如果为文件，则支持拖放
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

//当用户放下这个文件后，就会触发dropEvent事件
void Dialog::dropEvent(QDropEvent *event)
{
    //注意：这里如果有多文件存在，意思是用户一下子拖动了多个文件，而不是拖动一个目录
    //如果想读取整个目录，则在不同的操作平台下，自己编写函数实现读取整个目录文件名
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;


    //往文本框中追加文件名
    foreach (QUrl url, urls) {
        QString file_name = url.toLocalFile();
        if (file_name.endsWith(".xml")) {
            m_sFiles.insert(file_name);
            m_mFiles->setStringList(m_sFiles.toList());
        }
    }
}
