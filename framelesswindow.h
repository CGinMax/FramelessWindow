#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QMainWindow>
#define MARGIN_MIN_SIZE -3
#define MARGIN_MAX_SIZE 8

class FramelessWindow : public QMainWindow
{
    Q_OBJECT

    enum Direction {
        NONE,
        BOTTOMRIGHT,
        TOPRIGHT,
        TOPLEFT,
        BOTTOMLEFT,
        DOWN,
        LEFT,
        RIGHT,
        UP
    };

public:
    FramelessWindow(QWidget *parent = nullptr);
    ~FramelessWindow() override;

public:
    void updateRegion(QMouseEvent* event);
    void resizeRegion(int marginTop, int marginBottom,
                      int marginLeft, int marginRight);

public:
    bool event(QEvent *event) override;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    bool m_isPressed{false};
    bool m_isResizing{false};
    bool m_isDoublePressed{false};
    QPoint m_pressPoint;
    QPoint m_movePoint;
    Direction m_direction;
};

#endif // FRAMELESSWINDOW_H
