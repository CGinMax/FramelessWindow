#include "framelesswindow.h"
#include <QHoverEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QDebug>

FramelessWindow::FramelessWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_Hover);

//    QPushButton* btn = new QPushButton(this);
//    setCentralWidget(btn);

    QWidget* bg = new QWidget(this);
    setCentralWidget(bg);
    bg->setStyleSheet("background-color:#ffffff;border: 1px solid black;"
                      "border-radius: 15px;");

    setGeometry(300, 300, 400, 200);
}

FramelessWindow::~FramelessWindow()
{

}

void FramelessWindow::updateRegion(QMouseEvent *event)
{
    QRect mainRect = geometry();

    int marginTop = event->globalY() - mainRect.y();
    int marginBottom = mainRect.y() + mainRect.height() - event->globalY();
    int marginLeft = event->globalX() - mainRect.x();
    int marginRight = mainRect.x() + mainRect.width() - event->globalX();

    //计算四个方向的边距，记录鼠标状态
    if ( (marginRight >= MARGIN_MIN_SIZE && marginRight <= MARGIN_MAX_SIZE)
         && ((marginBottom <= MARGIN_MAX_SIZE) && marginBottom >= MARGIN_MIN_SIZE) ) {
        if (!m_isResizing) {
            m_direction = BOTTOMRIGHT;
        }
    } else if ( (marginTop >= MARGIN_MIN_SIZE && marginTop <= MARGIN_MAX_SIZE)
                && (marginRight >= MARGIN_MIN_SIZE && marginRight <= MARGIN_MAX_SIZE)) {
        if (!m_isResizing) {
            m_direction = TOPRIGHT;
        }
    } else if ( (marginLeft >= MARGIN_MIN_SIZE && marginLeft <= MARGIN_MAX_SIZE)
                && (marginTop >= MARGIN_MIN_SIZE && marginTop <= MARGIN_MAX_SIZE) ) {
        if (!m_isResizing) {
            m_direction = TOPLEFT;
        }
    } else if ( (marginLeft >= MARGIN_MIN_SIZE && marginLeft <= MARGIN_MAX_SIZE)
                && (marginBottom >= MARGIN_MIN_SIZE && marginBottom <= MARGIN_MAX_SIZE)) {
        if (!m_isResizing) {
            m_direction = BOTTOMLEFT;
        }
        m_isResizing = true;
    } else if (marginBottom <= MARGIN_MAX_SIZE && marginBottom >= MARGIN_MIN_SIZE) {
        if (!m_isResizing) {
            m_direction = DOWN;
        }
    } else if (marginLeft <= MARGIN_MAX_SIZE && marginLeft >= MARGIN_MIN_SIZE) {
        if (!m_isResizing) {
            m_direction = LEFT;
        }
    } else if (marginRight <= MARGIN_MAX_SIZE && marginRight >= MARGIN_MIN_SIZE) {
        if (!m_isResizing) {
            m_direction = RIGHT;
        }
    } else if (marginTop <= MARGIN_MAX_SIZE && marginTop >= MARGIN_MIN_SIZE) {
        if (!m_isResizing) {
            m_direction = UP;
        }
    } else {
        m_isResizing = false;
    }

    if (NONE != m_direction) {
        m_isResizing = true;
    }

    //调整区域，设置鼠标形状，如果鼠标按下则调整窗口大小
    resizeRegion(marginTop, marginBottom, marginLeft, marginRight);
}

void FramelessWindow::resizeRegion(int marginTop, int marginBottom, int marginLeft, int marginRight)
{
    switch (m_direction) {
    case BOTTOMRIGHT :
        setCursor(Qt::SizeFDiagCursor);
        if (m_isPressed) {
            QRect rect = geometry();
            rect.setBottomRight(rect.bottomRight() + m_movePoint);
            setGeometry(rect);
        }
        break;
    case BOTTOMLEFT:
        setCursor(Qt::SizeBDiagCursor);
        if (m_isPressed) {
            if (marginRight > minimumWidth() && marginTop > minimumHeight()) {
                QRect rect = geometry();
                rect.setBottomLeft(rect.bottomLeft() + m_movePoint);
                setGeometry(rect);
            }
        }
        break;
    case TOPLEFT:
        setCursor(Qt::SizeFDiagCursor);
        if (m_isPressed) {
            if (marginRight > minimumWidth() && marginBottom > minimumHeight()) {
                QRect rect = geometry();
                rect.setTopLeft(rect.topLeft() + m_movePoint);
                setGeometry(rect);
            }
        }
        break;
    case TOPRIGHT:
        setCursor(Qt::SizeBDiagCursor);
        if (m_isPressed) {
            if (marginLeft > minimumWidth() && marginBottom > minimumHeight()) {
                QRect rect = geometry();
                rect.setTopRight(rect.topRight() + m_movePoint);
                setGeometry(rect);
            }
        }
        break;
    case UP:
        setCursor(Qt::SizeVerCursor);
        if (m_isPressed) {

            if (marginBottom > minimumHeight()) {
                QRect rect = geometry();
                rect.setTop(rect.y() + m_movePoint.y());
                setGeometry(rect);
            }
        }
        break;
    case DOWN:
        setCursor(Qt::SizeVerCursor);
        if (m_isPressed) {
            QRect rect = geometry();
            rect.setHeight(rect.height() + m_movePoint.y());
            setGeometry(rect);
        }
        break;
    case LEFT:
        setCursor(Qt::SizeHorCursor);
        if (m_isPressed) {
            if (marginRight > minimumWidth()) {
                QRect rect = geometry();
                rect.setLeft(rect.x() + m_movePoint.x());
                setGeometry(rect);
            }
        }
        break;
    case RIGHT:
        setCursor(Qt::SizeHorCursor);
        if (m_isPressed) {
            QRect rect = geometry();
            rect.setWidth(rect.width() + m_movePoint.x());
            setGeometry(rect);
        }
        break;
    default:
        setCursor(Qt::ArrowCursor);
    }

    if (!m_isPressed) {
        m_isResizing = false;
        m_direction = NONE;
    }
}

bool FramelessWindow::event(QEvent *event)
{
    if (event->type() == QEvent::HoverMove) {
        QHoverEvent* hover_event = static_cast<QHoverEvent*>(event);
        QMouseEvent mouse_event(QEvent::MouseMove, hover_event->pos(), Qt::NoButton,
                                Qt::NoButton, Qt::NoModifier);
        mouseMoveEvent(&mouse_event);
    }

    return QWidget::event(event);
}

void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        m_pressPoint = event->globalPos();
    }

    return QWidget::mousePressEvent(event);
}

void FramelessWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed) {
        if (m_isResizing) {
            m_movePoint = event->globalPos() - m_pressPoint;
            m_pressPoint += m_movePoint;

            qDebug() << m_movePoint;
        }
        else {
//            if (m_isDoublePressed && windowState() == Qt::WindowMaximized) {
//                restoreWiget();
//            }
            QRect mainrect = geometry();
            int cur_y = m_pressPoint.y() - mainrect.y();
            if (cur_y > 0 && cur_y < 50) {
                QPoint point = event->globalPos() - m_pressPoint;
                move(pos() + point);
                m_pressPoint = event->globalPos();
            }

        }
    }

    if (windowState() != Qt::MaximumSize) {
        updateRegion(event);
    }
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        m_isPressed = false;
        m_isResizing = false;
        m_isDoublePressed = false;
    }

    QWidget::mouseReleaseEvent(event);
}

void FramelessWindow::leaveEvent(QEvent *event)
{
    //m_isPressed = false;
    m_isResizing = false;
    m_isDoublePressed = false;

    QWidget::leaveEvent(event);
}
