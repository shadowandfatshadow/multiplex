#ifndef SELECTCONTAINER_H
#define SELECTCONTAINER_H

#include <QDialog>
#include <QListWidgetItem>
#include <QSortFilterProxyModel>

#include "itempropertymodel.h"
#include "../svr/interactServer/interactwithserver.h"

namespace Ui {
class selectContainer;
}

class QActionGroup;
class QDateEdit;

class MySortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    MySortFilterProxyModel(QObject* parent =nullptr);

    QDate filterMinimumDate(){return minDate;}
    void setMinimumDate(QDate date);

    QDate filterMaximumDate(){return maxDate;}
    void setMaximumDate(QDate date);

    QString getCurUser(){return m_curUser;}
    void setCurUser(QString user);

    bool getIsShowUnDownloadPrj(){return m_isShowUnDownloadPrj;}
    void setIsShowUnDownloadPrj(bool flag);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

private:
    bool dateInRange(QDate date) const;
    bool prjNameValid(QString name) const;

    QDate   minDate;
    QDate   maxDate;
    QString m_curUser;
    bool    m_isShowUnDownloadPrj = false;

};


class selectContainer : public QDialog
{
    Q_OBJECT

public:
    explicit selectContainer(QWidget *parent = 0);
    ~selectContainer();
    void addItems2List(QStringList items);

    void initConnect();
    void initFilterPattern();
    void initUI();
    void setCalenderWidgetStyle(QDateEdit* de);             //设置日历样式
    void initModel();
    void addItems2Table(QMap<QString, SubItemInfo> items);
    void addSvrItem2Table(SvrPrjPrp svrPrj);
    void addProjectInfo(QAbstractItemModel* model,const QString prjName,const QDate createDate,const QString createUser);
    void updateProxyModel();                //添加完模型数据后更新其他控件，如代理模型、下拉框等

    //大小写敏感切换
    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(Qt::CaseSensitivity);

    //字符匹配模式切换
    QRegExp::PatternSyntax patternSyntax();
    void setPatternSyntax(QRegExp::PatternSyntax);
    QRegExp::PatternSyntax patternSyntaxFromAction(const QAction *a);

    QString m_curSelStr;
    static int m_useCnt;
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
private slots:
    void on_listWidget_containers_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_listWidget_containers_itemClicked(QListWidgetItem *item);

    void filterChange();

    void on_tv_prj_doubleClicked(const QModelIndex &index);

    void on_rb_showType_clicked(bool checked);

    void slotTreeMenu(const QPoint &pos);

    void slot_delete_prj();

signals:
#ifdef REQUIRE_SCENE
    void sceneChange(QString curScene);
#endif
private:
    Ui::selectContainer *ui;

    QStandardItemModel* m_sourceModel = nullptr;
    MySortFilterProxyModel* m_sortFilterProxyModel = nullptr;
    QSet<QString> m_createUsers;

    QAction *m_caseSensitivityAction;       //大小写敏感切换
    QActionGroup *m_patternGroup;           //字符匹配模式切换
};

#endif // SELECTCONTAINER_H
