#ifndef Mainwindow_hpp
#define Mainwindow_hpp

#include <QMainWindow>
#include <QMap>
#include <QList>
#include <Types.hpp>

class vector;

class QTabWidget;
class QLineEdit;
class QPushButton;
class QGraphicsScene;
class QAction;
class QGraphicsItem;

class Graph;
class ObstacleManager;

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onAddButtonClicked();
    void onTabCloseRequested(int index);
    void onShowDroneActionClicked();
    void onShowCarActionClicked();

private:
    QTabWidget* tabWidget;
    QLineEdit* tabNameInput;
    QPushButton* addButton;
    
    QAction* showDroneAction;
    QAction* showCarAction;

    struct TabGraphicsItems {
        QList<QGraphicsItem*> dronePathItems;  
        QList<QGraphicsItem*> carPathItems;    
    };
    
    QMap<int, TabGraphicsItems> tabItemsMap;    
    
    bool validateTabName(QString const& name);
    void createNewTab(QString const& name);

    bool process(QString const& path, QGraphicsScene* scene, int tabIndex);
    void drawGraph(Graph const* g, QGraphicsScene* scene);
    void drawObstacleManager(ObstacleManager const* g, QGraphicsScene* scene);
    QList<QGraphicsItem*> drawPolyLine(std::vector<Point> const& points, QGraphicsScene* scene, QPen const& pen, bool isClosed = false);
    void drawSegments(std::vector<std::pair<Point, Point>> const& segments, QGraphicsScene* scene, cord width);

    void updateAllTabsVisibility();
    void updateTabVisibility(int tabIndex);
    void addDronePathItem(QGraphicsItem* item, int tabIndex);
    void addCarPathItem(QGraphicsItem* item, int tabIndex);
};

#endif /* Mainwindow_hpp */