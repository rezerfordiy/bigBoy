#ifndef Mainwindow_hpp
#define Mainwindow_hpp

#include <QMainWindow>

#include <Types.hpp>

class vector;

class QTabWidget;
class QLineEdit;
class QPushButton;
class QGraphicsScene;

class Graph;
class ObstacleManager;

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onAddButtonClicked();
    void onTabCloseRequested(int index);

private:
    QTabWidget* tabWidget;
    QLineEdit* tabNameInput;
    QPushButton* addButton;
    
    bool validateTabName(QString const& name);
    void createNewTab(QString const& name);

    bool process(QString const& path, QGraphicsScene* scene);
    void drawGraph(Graph const* g, QGraphicsScene* scene);
    void drawObstacleManager(ObstacleManager const* g, QGraphicsScene* scene);
    void drawPolyLine(std::vector<Point> const& points, QGraphicsScene* scene, int type = -1);

    void drawSegments(std::vector<std::pair<Point, Point>> const& segments, QGraphicsScene* scene, cord width);


};

#endif /* Mainwindow_hpp */