#include "selectcontainer.h"
#include "ui_selectcontainer.h"
#include <windows.h>
#include <windowsx.h>
#include <QMenu>
#include <QActionGroup>
#include <QBitmap>
#include <QCompleter>
#include <QPainter>
#include <QCalendarWidget>

int selectContainer::m_useCnt = 0;

selectContainer::selectContainer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectContainer)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);// | Qt::Popup

    //ui->pushButton_ok->setEnabled(false);
    m_curSelStr = "";
    initModel();
    initUI();
    initFilterPattern();
    initConnect();
}

selectContainer::~selectContainer()
{
    delete ui;
}

void selectContainer::addItems2List(QStringList items)
{
//    ui->listWidget_containers->addItems(items);
    //    ui->listWidget_containers->setCurrentRow(0);
}

void selectContainer::initConnect()
{
    connect(ui->le_filterText, &QLineEdit::textChanged, this, &selectContainer::filterChange);
    connect(m_caseSensitivityAction, &QAction::toggled, this, &selectContainer::filterChange);
    connect(m_patternGroup, &QActionGroup::triggered, this, &selectContainer::filterChange);

    connect(ui->de_fromDate, &QDateTimeEdit::dateChanged,[&](){
        m_sortFilterProxyModel->setMinimumDate(ui->de_fromDate->date());
        ui->de_toDate->setMinimumDate(ui->de_fromDate->date());
    });
    connect(ui->de_toDate, &QDateTimeEdit::dateChanged,[&](){
        m_sortFilterProxyModel->setMaximumDate(ui->de_toDate->date());
        ui->de_fromDate->setMaximumDate(ui->de_toDate->date());
    });

    connect(ui->cbb_createUser,QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            [&](const QString& str) {
        m_sortFilterProxyModel->setCurUser(str);
    });

    //右键菜单
    ui->tv_prj->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tv_prj,&QTreeView::customContextMenuRequested,this,&selectContainer::slotTreeMenu);
}

void selectContainer::initFilterPattern()
{
    QMenu *menu = new QMenu(this);
    m_caseSensitivityAction = menu->addAction(QStringLiteral("区分大小写"));
    m_caseSensitivityAction->setCheckable(true);
    menu->addSeparator();
    m_patternGroup = new QActionGroup(this);
    m_patternGroup->setExclusive(true);
    QAction *patternAction = menu->addAction(QStringLiteral("普通字符匹配"));
    patternAction->setData(QVariant(int(QRegExp::FixedString)));
    patternAction->setCheckable(true);
    patternAction->setChecked(true);
    m_patternGroup->addAction(patternAction);
    patternAction = menu->addAction(QStringLiteral("正则匹配"));
    patternAction->setCheckable(true);
    patternAction->setData(QVariant(int(QRegExp::RegExp2)));
    m_patternGroup->addAction(patternAction);

    menu->setStyleSheet(QString("\
                    QMenu{\
                    background-color:white;\
                        border:1px solid #eeeeee;\
                    }\
                    QMenu::item{\
                        font:13px \"黑体\";\
                        margin:2px 2px;\
                    }\
                    QMenu::item:selected{\
                        background-color:#1E90FF;\
                    }\
                    QMenu::indicator{\
                        width:30px;\
                        height:30px;\
                    }\
                    QMenu::indicator:checked{\
                        image:url(:/image/image/checkedCircle.png);\
                    }"));

    ui->btn_filterPattern->setMenu(menu);
    ui->btn_filterPattern->setPopupMode(QToolButton::InstantPopup);

    ui->le_filterText->setPlaceholderText(QStringLiteral("请输入项目名"));
}

void selectContainer::initUI()
{
    //圆角
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(bmp.rect(), 20, 20);
    setMask(bmp);

    ui->le_filterText->setClearButtonEnabled(true);
    //自动补全，没必要
//    QCompleter* comp = new QCompleter(this);
//    QAbstractItemView *popup = comp->popup();
//    popup->setStyleSheet("background-color:white;");
//    comp->setModel(m_sourceModel);
//    ui->le_filterText->setCompleter(comp);


    QCompleter *pCompleter = new QCompleter(ui->cbb_createUser->model(), this);
    ui->cbb_createUser->setCompleter(pCompleter);

    ui->tv_prj->setRootIsDecorated(false);
    ui->tv_prj->setAlternatingRowColors(true);
    ui->tv_prj->setModel(m_sortFilterProxyModel);
    ui->tv_prj->setSortingEnabled(true);
    ui->tv_prj->sortByColumn(1, Qt::DescendingOrder);              //默认以日期排序
    ui->tv_prj->header()->setMinimumSectionSize(60);               //设置最小列宽

    setCalenderWidgetStyle(ui->de_fromDate);
    setCalenderWidgetStyle(ui->de_toDate);
}

void selectContainer::setCalenderWidgetStyle(QDateEdit *de)
{
    de->setCalendarPopup(true);
    QCalendarWidget* calenderWidget = de->calendarWidget();
    calenderWidget->setStyleSheet(QString("#qt_calendar_navigationbar {\
                                          background: rgb(250, 250, 250);\
                                        }\
                                        QAbstractItemView {\
                                            color: black;\
                                            selection-color: white;\
                                            selection-background-color:rgb(149, 191, 255);\
                                            font: 12px;\
                                            background:white;\
                                        }"));
}

void selectContainer::initModel()
{
    m_sourceModel = new QStandardItemModel(0,3);
    m_sortFilterProxyModel = new MySortFilterProxyModel(this);

    m_sourceModel->setHeaderData(0, Qt::Horizontal, QStringLiteral("项目名称"));
    m_sourceModel->setHeaderData(1, Qt::Horizontal, QStringLiteral("创建日期"));
    m_sourceModel->setHeaderData(2, Qt::Horizontal, QStringLiteral("创建者"));
}

void selectContainer::addItems2Table(QMap<QString, SubItemInfo> items)
{
    for(auto it : items)
    {
        addProjectInfo(m_sourceModel,it.name,QDate::fromString(it.createDate,"yyyy-MM-dd"),it.createUser);
        m_createUsers.insert(it.createUser);
    }
}

void selectContainer::addSvrItem2Table(SvrPrjPrp svrPrj)
{
    QString prjName = QString("svr:%1").arg(svrPrj.name);
    QDateTime createTime = QDateTime::fromString(svrPrj.createDate,"yyyy-MM-dd hh:mm:ss");
    addProjectInfo(m_sourceModel,prjName,createTime.date(),svrPrj.createUser);
    m_createUsers.insert(svrPrj.createUser);

}

void selectContainer::addProjectInfo(QAbstractItemModel *model, const QString prjName, const QDate createDate,const QString createUser)
{
    m_sourceModel->insertRow(0);
    m_sourceModel->setData(m_sourceModel->index(0,0),prjName);
    m_sourceModel->setData(m_sourceModel->index(0,1),createDate);
    m_sourceModel->setData(m_sourceModel->index(0,2),createUser);
}

void selectContainer::updateProxyModel()
{
    m_sortFilterProxyModel->setSourceModel(m_sourceModel);
    ui->cbb_createUser->clear();
    ui->cbb_createUser->addItem(QStringLiteral("全部"));
    ui->cbb_createUser->addItems(m_createUsers.toList());
    ui->cbb_createUser->setCurrentText(QStringLiteral("全部"));

    for (int i = 0; i < m_sortFilterProxyModel->columnCount(); ++i)
        ui->tv_prj->resizeColumnToContents(i);

    ui->tv_prj->setCurrentIndex(m_sortFilterProxyModel->index(0,0));
}

Qt::CaseSensitivity selectContainer::caseSensitivity() const
{
    return m_caseSensitivityAction->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
}

void selectContainer::setCaseSensitivity(Qt::CaseSensitivity cs)
{
    m_caseSensitivityAction->setChecked(cs == Qt::CaseSensitive);
}

QRegExp::PatternSyntax selectContainer::patternSyntax()
{
    return patternSyntaxFromAction(m_patternGroup->checkedAction());
}

void selectContainer::setPatternSyntax(QRegExp::PatternSyntax s)
{
    const QList<QAction*> actions = m_patternGroup->actions();
    for (QAction *a : actions) {
        if (patternSyntaxFromAction(a) == s) {
            a->setChecked(true);
            break;
        }
    }
}

QRegExp::PatternSyntax selectContainer::patternSyntaxFromAction(const QAction *a)
{
    return static_cast<QRegExp::PatternSyntax>(a->data().toInt());
}

bool selectContainer::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG*)message;
    if (msg->message == WM_NCHITTEST)
    {
//暂未使用
//        int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
        int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
        if(yPos < 35)
        {
            *result = HTCAPTION;
            return true;
        }
    }
    return false;
}

void selectContainer::on_listWidget_containers_itemDoubleClicked(QListWidgetItem *item)
{
    if(item == nullptr)
        return;
    on_pushButton_ok_clicked();
}

void selectContainer::on_tv_prj_doubleClicked(const QModelIndex &index)
{
    on_pushButton_ok_clicked();
}


void selectContainer::on_pushButton_cancel_clicked()
{
    m_curSelStr.clear();
    close();
}

void selectContainer::on_pushButton_ok_clicked()
{
//    m_curSelStr = ui->listWidget_containers->currentItem()->text();
    int row = ui->tv_prj->currentIndex().row();
    QAbstractItemModel* model = ui->tv_prj->model();
    QModelIndex index = model->index(row,0);
    m_curSelStr = model->data(index).toString();
    close();
}

void selectContainer::on_listWidget_containers_itemClicked(QListWidgetItem *item)
{
    ui->pushButton_ok->setEnabled(true);
}

void selectContainer::on_rb_showType_clicked(bool checked)
{
    m_sortFilterProxyModel->setIsShowUnDownloadPrj(checked);
}

void selectContainer::slotTreeMenu(const QPoint &pos)
{
    QMenu menu;
    QModelIndex curIndex = ui->tv_prj->indexAt(pos);
    QModelIndex index = curIndex.sibling(curIndex.row(),0);
    if (index.isValid())
    {
        if(ui->tv_prj->model()->data(index).toString().mid(0,4) != QStringLiteral("svr:"))
        {
            menu.addAction(QStringLiteral("删除"), this, SLOT(slot_delete_prj()));
        }
    }
    menu.exec(QCursor::pos());  //显示菜单
}

void selectContainer::slot_delete_prj()
{
    //讨论结果：只删除xml文件中的索引就可以了
    //删除xml中对象
    int row = ui->tv_prj->currentIndex().row();
    QAbstractItemModel* model = ui->tv_prj->model();
    QModelIndex index = model->index(row,0);
    m_curSelStr = model->data(index).toString();

    model->removeRow(index.row());

    QFile file(itemPropertyModel::m_pTopContainerPrpModel->m_prpFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();       //需要关掉

    QDomElement root = doc.documentElement();
    QDomElement subItemsNode = root.firstChildElement("subItems");
    if(subItemsNode.isNull())
    {
        return;
    }

    QDomNodeList list=subItemsNode.elementsByTagName("item");
    for(int i=0;i<list.count();i++)
    {
        QDomElement e=list.at(i).toElement();
        if(e.attribute("name") == m_curSelStr)
        {
            subItemsNode.removeChild(list.at(i));
            break;
        }
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
            return;
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();

    //删除用来构建项目选择框中数据模型的信息
    itemPropertyModel::m_pTopContainerPrpModel->m_subItemsName.removeOne(m_curSelStr);
    itemPropertyModel::m_pTopContainerPrpModel->m_subItems.remove(m_curSelStr);


}

void selectContainer::filterChange()
{
    //特殊需求，正则表达的 * 代表的意思是 .*
    QRegExp regExp;
    if(patternSyntax() == QRegExp::RegExp2)
    {
        QString replaceStr = ui->le_filterText->text();
        replaceStr.replace("*",".*");
        regExp = QRegExp(replaceStr,
                        caseSensitivity(),
                        patternSyntax());
    }
    else
    {
        regExp = QRegExp(ui->le_filterText->text(),
                        caseSensitivity(),
                        patternSyntax());
    }

    m_sortFilterProxyModel->setFilterRegExp(regExp);
}

#pragma region MySortFilterProxyModel{

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent)
    :QSortFilterProxyModel(parent)
{

}

void MySortFilterProxyModel::setMinimumDate(QDate date)
{
    minDate = date;
    invalidateFilter();
}

void MySortFilterProxyModel::setMaximumDate(QDate date)
{
    maxDate = date;
    invalidateFilter();
}

void MySortFilterProxyModel::setCurUser(QString user)
{
    m_curUser = user;
    invalidateFilter();
}

void MySortFilterProxyModel::setIsShowUnDownloadPrj(bool flag)
{
    m_isShowUnDownloadPrj = flag;
    invalidateFilter();
}

bool MySortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index0 = sourceModel()->index(source_row,0,source_parent);          //项目名
    QModelIndex index1 = sourceModel()->index(source_row,1,source_parent);          //创建时间
    QModelIndex index2 = sourceModel()->index(source_row,2,source_parent);          //创建者

    //创建日期过滤
    bool isDateInRange = sourceModel()->data(index1).toString().isEmpty() || dateInRange(sourceModel()->data(index1).toDate());

    //创建者过滤
    bool isFilterUser = (m_curUser == QStringLiteral("全部")) ? true : (sourceModel()->data(index2).toString() == m_curUser);

    //是否显示未下载项目的过滤
    bool isShow = m_isShowUnDownloadPrj ? true : prjNameValid(sourceModel()->data(index0).toString());

    //模糊搜索的过滤
    return sourceModel()->data(index0).toString().contains(filterRegExp())
            && isFilterUser
            && isDateInRange
            && isShow;
}

bool MySortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QVariant leftData = sourceModel()->data(source_left);
    QVariant rightData = sourceModel()->data(source_right);

    if(leftData.type() == QVariant::Date)
    {
        return leftData.toDate() < rightData.toDate();
    }
    else
    {
        QString leftString = leftData.toString();
        QString rightString = rightData.toString();
        return QString::localeAwareCompare(leftString,rightString) < 0;
    }
}

bool MySortFilterProxyModel::dateInRange(QDate date) const
{
    return (!minDate.isValid() || date >= minDate) && (!maxDate.isValid() || date <= maxDate);
}

bool MySortFilterProxyModel::prjNameValid(QString name) const
{
    //暂时以名称是否以 svn: 开头区分本地项目与未下载项目
    return (name.mid(0,4) != "svr:");
}

#pragma endregion }
