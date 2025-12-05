#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QFile>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QPen>
#include <QMenu>
#include <QAction>
#include <QMenuBar>

#include "Mainwindow.hpp"
#include "Solver.hpp"
#include "CarPathPlanner.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                        tabWidget(new QTabWidget(this)), 
                                        tabNameInput(new QLineEdit(this)), 
                                        addButton(new QPushButton("add config", this)) {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(tabNameInput);
    controlLayout->addWidget(addButton);
    
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(tabWidget);

    QMenu* viewMenu = menuBar()->addMenu("View");
    
    showDroneAction = new QAction("Show Drone", this);
    showDroneAction->setCheckable(true);
    showDroneAction->setChecked(true);
    
    showCarAction = new QAction("Show Car", this);
    showCarAction->setCheckable(true);
    showCarAction->setChecked(true);
    
    viewMenu->addAction(showDroneAction);
    viewMenu->addAction(showCarAction);
    
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(tabNameInput, &QLineEdit::returnPressed, this, &MainWindow::onAddButtonClicked);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
    connect(showDroneAction, &QAction::triggered, this, &MainWindow::onShowDroneActionClicked);
    connect(showCarAction, &QAction::triggered, this, &MainWindow::onShowCarActionClicked);
    
    tabWidget->setTabsClosable(true);
}

void MainWindow::onShowDroneActionClicked() {
    updateAllTabsVisibility();
}

void MainWindow::onShowCarActionClicked() {
    updateAllTabsVisibility();
}

void MainWindow::updateAllTabsVisibility() {
    for (int i = 0; i < tabWidget->count(); ++i) {
        updateTabVisibility(i);
    }
}

void MainWindow::updateTabVisibility(int tabIndex) {
    if (!tabItemsMap.contains(tabIndex)) {
        return;
    }
    
    TabGraphicsItems& items = tabItemsMap[tabIndex];
    
    bool showDrone = showDroneAction->isChecked();
    bool showCar = showCarAction->isChecked();
    
    for (QGraphicsItem* item : items.dronePathItems) {
        if (item) item->setVisible(showDrone);
    }
    for (QGraphicsItem* item : items.carPathItems) {
        if (item) item->setVisible(showCar);
    }
}

void MainWindow::addDronePathItem(QGraphicsItem* item, int tabIndex) {
    if (!tabItemsMap.contains(tabIndex)) {
        tabItemsMap[tabIndex] = TabGraphicsItems();
    }
    tabItemsMap[tabIndex].dronePathItems.append(item);
    item->setVisible(showDroneAction->isChecked());
}

void MainWindow::addCarPathItem(QGraphicsItem* item, int tabIndex) {
    if (!tabItemsMap.contains(tabIndex)) {
        tabItemsMap[tabIndex] = TabGraphicsItems();
    }
    tabItemsMap[tabIndex].carPathItems.append(item);
    item->setVisible(showCarAction->isChecked());
}

void MainWindow::onAddButtonClicked() {
    QString name = tabNameInput->text().trimmed();
    
    if (name.isEmpty()) {
        QMessageBox::warning(this, "error", "empty string");
        return;
    }
    
    if (!validateTabName(name)) {
        return;
    }
    
    createNewTab(name);
    tabNameInput->clear();
}

void MainWindow::onTabCloseRequested(int index) {
    if (index >= 0) {
        tabItemsMap.remove(index);
        
        QMap<int, TabGraphicsItems> newMap;
        for (auto it = tabItemsMap.begin(); it != tabItemsMap.end(); it++) {
            int oldIndex = it.key();
            if (oldIndex > index) {
                newMap[oldIndex - 1] = it.value();
            } else if (oldIndex < index) {
                newMap[oldIndex] = it.value();
            }
        }
        tabItemsMap = newMap;
        
        QWidget* widget = tabWidget->widget(index);
        tabWidget->removeTab(index);
        delete widget; 
    }
}

bool MainWindow::validateTabName(QString const& name) {
    if (!QFile::exists(name)) {
         QMessageBox::warning(this, "error", "this config not exists");
        return false;
    }
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabWidget->tabText(i) == name) {
            QMessageBox::warning(this, "error", "this config already exists");
            return false;
        }
    }
    return true;
}

void MainWindow::createNewTab(QString const& name) {
    QWidget* tabContent = new QWidget(tabWidget);
    QVBoxLayout* tabLayout = new QVBoxLayout(tabContent);
    
    QGraphicsScene* scene = new QGraphicsScene(tabContent);
    QGraphicsView* view = new QGraphicsView(scene);
    
    tabLayout->addWidget(view);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    
    int tabIndex = tabWidget->addTab(tabContent, name);
    
    process(name, scene, tabIndex);
    
    QRectF sceneRect = scene->itemsBoundingRect();
    sceneRect.adjust(-10, -10, 10, 10); 
    view->fitInView(sceneRect, Qt::KeepAspectRatio);
}

bool MainWindow::process(QString const& path, QGraphicsScene* scene, int tabIndex) {
    constexpr static int AMOUNT = 100;
    std::string standart = path.toStdString();
    Solver slv = Solver(standart);

    PathData::TaskType type = slv.getData().getTaskType();
    if (type == PathData::TaskType::PATHFINDING) {
        drawObstacleManager(&slv.getData().getObstacleManager(), scene);
        drawGraph(&(dynamic_cast<CarPathPlanner const*>(slv.getPathFindingTask()[PathFindingTask::Type::CAR])->getGraphManager().graph), scene);

        std::vector<Point> carPoints = slv.getPathFindingTask().getPointsByType(PathFindingTask::Type::CAR);
        
        QPen greenPen(Qt::green);
        greenPen.setWidth(5);
        QList<QGraphicsItem*> carPathItems = drawPolyLine(carPoints, scene, greenPen);
        for (QGraphicsItem* item : carPathItems) {
            addCarPathItem(item, tabIndex);
        }

        std::vector<Point> dronePoints = slv.getPathFindingTask().getPointsByType(PathFindingTask::Type::DRONE);
        QPen bluePen(Qt::red);
        bluePen.setWidth(3);
        QList<QGraphicsItem*> dronePathItems = drawPolyLine(dronePoints, scene, bluePen);
        for (QGraphicsItem* item : dronePathItems) {
            addDronePathItem(item, tabIndex);
        }

    } else if (type == PathData::TaskType::CUTFINDING) {
        drawPolyLine(slv.getData().getDestinations(), scene, QPen(Qt::darkGreen), true);
        drawSegments(slv.getTask2().getPathPoint(), scene, slv.getData().getWorkWidth());
    } else {
        QMessageBox::warning(this, "data error", "unknown type");
    }
    return true;
}

void MainWindow::drawGraph(Graph const* graph, QGraphicsScene* scene) {
    for (const auto& node : graph->nodes) {
        QGraphicsEllipseItem* point = new QGraphicsEllipseItem(node.x - 2, node.y - 2, 4, 4);
        point->setBrush(Qt::blue);
        point->setPen(Qt::NoPen);
        scene->addItem(point);
    }
    
    // maybe skip through one
    for (const auto& edge : graph->edges) {
        cord x1 = graph->nodes[edge.from].x;
        cord y1 = graph->nodes[edge.from].y;
        cord x2 = graph->nodes[edge.to].x;
        cord y2 = graph->nodes[edge.to].y;
        
        QGraphicsLineItem* line = new QGraphicsLineItem(x1, y1, x2, y2);
        QPen pen(Qt::blue);
        pen.setWidth(1);
        line->setPen(pen);
        scene->addItem(line);
    }
}

void MainWindow::drawObstacleManager(ObstacleManager const* manager, QGraphicsScene* scene) {
    for (auto const& ob : manager->get()) {
        QGraphicsRectItem* rect = new QGraphicsRectItem(ob.x1, ob.y1, ob.x2 - ob.x1, ob.y2 - ob.y1);
        rect->setBrush(Qt::red);
        scene->addItem(rect);
    }
}


QList<QGraphicsItem*> MainWindow::drawPolyLine(std::vector<Point> const& points, QGraphicsScene* scene, QPen const& pen, bool isClosed) {
    QList<QGraphicsItem*> items;
    
    if (points.empty() || scene == nullptr) {
        return items;
    }
    
    QPainterPath path;
    
    if (!points.empty()) {
        path.moveTo(points[0].x(), points[0].y());
    }
    
    for (size_t i = 1; i < points.size(); ++i) {
        path.lineTo(points[i].x(), points[i].y());
    }
    if (isClosed) {
        path.lineTo(points[0].x(), points[0].y());
    }
    
    QGraphicsPathItem* pathItem = scene->addPath(path);
    pathItem->setPen(pen);
    items.append(pathItem);
    
    return items;
}

void MainWindow::drawSegments(std::vector<std::pair<Point, Point>> const& segments, QGraphicsScene* scene, cord width) {
    
    QPen segmentPen(Qt::black);
    segmentPen.setWidth(2);
    
    QPen rectPen(Qt::red);
    rectPen.setWidth(1);
    QBrush rectBrush(QColor(255, 0, 0, 50));
    
    const cord rectSize = width;
    
    for (const auto& segment : segments) {
        Point p1 = segment.first;
        Point p2 = segment.second;
        
        QGraphicsLineItem* line = scene->addLine(p1.x(), p1.y(), p2.x(), p2.y(), segmentPen);
        
        if (p1.y() == p2.y()) {
            cord leftX = std::min(p1.x(), p2.x());
            cord rightX = std::max(p1.x(), p2.x());
            cord y = p1.y();
            
            cord rectY = y - rectSize / 2;
            QGraphicsRectItem* rect = scene->addRect(leftX, rectY, rightX - leftX, rectSize, rectPen, rectBrush);
        }
        else if (p1.x() == p2.x()) {
            cord topY = std::min(p1.y(), p2.y());
            cord bottomY = std::max(p1.y(), p2.y());
            cord x = p1.x();
            
            cord rectX = x - rectSize / 2;
            QGraphicsRectItem* rect = scene->addRect(rectX, topY, rectSize, bottomY - topY, rectPen, rectBrush);
        }
        else {
            qWarning() << "Segment is neither horizontal nor vertical!";
            continue;
        }
    }
}
