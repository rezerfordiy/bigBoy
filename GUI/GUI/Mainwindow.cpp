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

#include "Mainwindow.hpp"
#include "Solver.hpp"

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
    
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(tabNameInput, &QLineEdit::returnPressed, this, &MainWindow::onAddButtonClicked);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
    
    tabWidget->setTabsClosable(true);
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
    QGraphicsScene* scene = new QGraphicsScene(this);
    
    process(name, scene);
    
    QGraphicsView* view = new QGraphicsView(scene);
    
    QRectF sceneRect = scene->itemsBoundingRect();
    sceneRect.adjust(-10, -10, 10, 10); 
    view->fitInView(sceneRect, Qt::KeepAspectRatio);
    tabWidget->addTab(view, name);
}

bool MainWindow::process(QString const& path, QGraphicsScene* scene) {
    const static int AMOUNT = 100;
    std::string standart = path.toStdString();
    Solver slv = Solver(standart);

    int type = slv.data.getTaskType();
    if (type == 1) {
        drawObstacleManager(&slv.task1.getData().getObstacleManager(), scene);
        drawGraph(&slv.task1.getGraphManager().graph, scene);

        std::vector<Point> points;
        for (int i = 0; i < slv.task1.getPathDubins().size(); i++) {
            DubinsPath dubin = slv.task1.getPathDubins()[i];
            cord length = dubin.length();
            if (length > 0) {
                for (int j = 0; j < AMOUNT; j++) {
                    cord t = (length * j) / AMOUNT;      
                    DubinsConfiguration conf = dubin.sample(t);
                    points.emplace_back(conf.x, conf.y);
                }
            }
        }
        drawPolyLine(points, scene);

    } else if (type == 2) {
        drawPolyLine(slv.data.getDestinations(), scene, 2);
        drawSegments(slv.task2.getPathPoint(), scene, slv.data.getWorkWidth());
    } else {
        QMessageBox::warning(this, "data error", "unknown type");

    }
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


void MainWindow::drawPolyLine(std::vector<Point> const& points, QGraphicsScene* scene, int type) {
    if (points.empty() || scene == nullptr) {
        return;
    }
    
    QPainterPath path;
    
    if (!points.empty()) {
        path.moveTo(points[0].x(), points[0].y());
    }
    
    for (size_t i = 1; i < points.size(); ++i) {
        path.lineTo(points[i].x(), points[i].y());
    }
    if (type == 2) {
        path.lineTo(points[0].x(), points[0].y());
    }
    
    QGraphicsPathItem* pathItem = scene->addPath(path);
    
    QPen greenPen(Qt::green);
    greenPen.setWidth(5); 
    
    pathItem->setPen(greenPen);
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