#include "glwidget.h"

GlWidget::GlWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    mIsCountPoint = false;
    mIsCountTest = false;
    m_wScreen = 600;
    m_hScreen = 600;
    m_wIndent = 50;
    m_hIndent = 50;
    m_wChart = m_wScreen - m_wIndent * 2;
    m_hChart = m_hScreen - m_hIndent * 2;

    float x;
    m_wScChart = m_wChart / N_X;
    for(int i=0; i<=N_X; i+=10) {
        x = m_wScChart * i;
        i != N_X ? m_xTitles.push_back(QPair<float, QString>(x, QString::number(i))) : m_xTitles.push_back(QPair<float, QString>(x, "X"));
    }

    float y;
    m_hScChart = m_hChart / N_Y;
    for(int i=10; i<=N_Y; i+=10) {
        y = m_hScChart * i;
        i != N_Y ? m_yTitles.push_back(QPair<float, QString>(y, QString::number(i))) : m_yTitles.push_back(QPair<float, QString>(y, "Y"));
    }
}

void GlWidget::setParameters(QStringList list)
{
    if(list.size() == 10) {
        for(int i = 0; i < list.size(); i++) {
            if(list[i] == "x1:" && i < (list.size() - 1)) {
                mParameter.x1 = list[i+1].toFloat();
            }
            else if(list[i] == "x2:" && i < (list.size() - 1)) {
                mParameter.x2 = list[i+1].toFloat();
            }
            else if(list[i] == "z1:" && i < (list.size() - 1)) {
                mParameter.y1 = list[i+1].toFloat();
            }
            else if(list[i] == "z2:" && i < (list.size() - 1)) {
                mParameter.y2 = list[i+1].toFloat();
            }
            else if(list[i] == "rotation:" && i < (list.size() - 1)) {
                mParameter.ang = list[i+1].toFloat();
            }
        }
    }

    counting();
}

void GlWidget::checkLine(QStringList list)
{
    mShowString = list;
    if(list.size() >= 8) {
        for(int i = 0; i < list.size(); i++) {
            if(list[i] == "x1:" && i < (list.size() - 1)) {
                mParameter.x1 = list[i+1].toFloat();
            }
            else if(list[i] == "x2:" && i < (list.size() - 1)) {
                mParameter.x2 = list[i+1].toFloat();
            }
            else if(list[i] == "z1:" && i < (list.size() - 1)) {
                mParameter.y1 = list[i+1].toFloat();
            }
            else if(list[i] == "rotation:" && i < (list.size() - 1)) {
                mParameter.ang = list[i+1].toFloat();
            }
        }
    }

    countingTest();
}

void GlWidget::initializedGL()
{
    glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
}

void GlWidget::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_hScreen, m_wScreen, 0, -1, 1);

    glViewport(0, 0, (GLint)w, (GLint)h);
    makeRasterFont();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    m_wSc = m_wScreen / (qreal)w;
    m_hSc = m_hScreen / (qreal)h;
}

void GlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.0, 0.0, 0.0);

    showCounting();
//    float y = 20;
//    for(auto str : mShowString) {
//        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());
//        y += 20;
//    }
}

void GlWidget::makeRasterFont()
{
    GLubyte space[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    GLubyte letters[][13] = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36},
        {0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00},
        {0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18},
        {0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70},
        {0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e},
        {0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c},
        {0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30},
        {0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00},
        {0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03},
        {0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c},
        {0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18},
        {0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e},
        {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e},
        {0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c},
        {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
        {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
        {0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff},
        {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
        {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
        {0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06},
        {0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60},
        {0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e},
        {0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
        {0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
        {0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
        {0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
        {0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
        {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
        {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
        {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
        {0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
        {0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
        {0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
        {0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
        {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
        {0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
        {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
        {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
        {0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
        {0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
        {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
        {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
        {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
        {0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
        {0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
        {0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
        {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
        {0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff},
        {0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c},
        {0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60},
        {0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18},
        {0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70},
        {0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
        {0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03},
        {0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e},
        {0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0},
        {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00},
        {0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00},
        {0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0},
        {0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78},
        {0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00},
        {0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00},
        {0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00},
        {0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
        {0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f},
        {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
        {0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00}
    };

    GLuint i, j;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    fontOffset = glGenLists(128);

    for(i=32; i<127; i++) {
        glNewList(fontOffset + i, GL_COMPILE);
        glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, letters[i - 32]);
        glEndList();
    }

    glNewList(fontOffset + ' ', GL_COMPILE);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, space);
    glEndList();
}

void GlWidget::printString(float x, float y, const char *s)
{
    glRasterPos2f(x, y);

    glPushAttrib(GL_LIST_BIT);
    glListBase(fontOffset);
    glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte*) s);
    glPopAttrib();
}

void GlWidget::drawGrid()
{
    glBegin(GL_LINES);
       glVertex2f(m_wIndent, m_hScreen - m_hIndent);
       glVertex2f(m_wIndent, m_hScreen - (m_hChart + m_hIndent));
       glVertex2f(m_wIndent, m_hScreen - m_hIndent);
       glVertex2f(m_wIndent + m_wChart, m_hScreen - m_hIndent);
    glEnd();

    float fW = m_wChart / N_X;
    float fH = m_hChart / N_Y;

    glLineStipple(3, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);

    glBegin(GL_LINES);
        for(int i=10; i<=N_X; i+=10) {
            glVertex2f(m_wIndent + i * fW, m_hScreen - m_hIndent);
            glVertex2f(m_wIndent + i * fW, m_hScreen - (m_hChart + m_hIndent));
        }
        for(int i=10; i<=N_Y; i+=10) {
            glVertex2f(m_wIndent, m_hScreen - m_hIndent - i * fH);
            glVertex2f(m_wIndent + m_wChart, m_hScreen - m_hIndent - i * fH);
        }
    glEnd();

    glDisable(GL_LINE_STIPPLE);
}

void GlWidget::drawTitles()
{
    float xPos = 0;
    float yPos = m_hScreen - m_hIndent + 15 * m_hSc;

    for(int i=0, s=m_xTitles.size(); i<s; i++) {
        xPos = m_wIndent + m_xTitles[i].first - 5;
        printString(xPos, yPos, m_xTitles[i].second.toStdString().c_str());
    }

    xPos = m_wIndent - 20 * m_wSc;
    for(int i=0, s=m_yTitles.size(); i<s; i++) {
        yPos = m_hScreen - m_hIndent - m_yTitles[i].first + 5;
        printString(xPos, yPos, m_yTitles[i].second.toStdString().c_str());
    }
}

void GlWidget::showCounting()
{
    float y = 20;
    QString str;
    str = "x1: " + QString::number(mParameter.x1, 'f', 2) + ". z1: " + QString::number(mParameter.y1, 'f', 2) +
            ". x2: " + QString::number(mParameter.x2, 'f', 2) + ". z2: " + QString::number(mParameter.y2, 'f', 2) +
            ". ang: " + QString::number(mParameter.ang, 'f', 2);
    printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

    if(mIsCountPoint) {
        str = "k = tg(90 - ang) = " + QString::number(mParameter.k, 'f', 2);
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "b = z1 - k * x1 = " + QString::number(mParameter.b, 'f', 2);
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "k2 = -1 / k = " + QString::number(mParameter.k2, 'f', 2);
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "b2 = x1 / k + z1 = " + QString::number(mParameter.b2, 'f', 2);
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "z3 = (b - k * b2 / k2) / (1 - k / k2)";
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "z3 = (" + QString::number(mParameter.b, 'f', 2) + " - " + QString::number(mParameter.k, 'f', 2) +
                " * " + QString::number(mParameter.b2, 'f', 2) + " / " + QString::number(mParameter.k2, 'f', 2) +
                ") / (1 - " + QString::number(mParameter.k, 'f', 2) + " / " + QString::number(mParameter.k2, 'f', 2) + ")";
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());
//        str = "z3 = (x2 / tg(90 - ang) + z2 - (tg(90 - ang)*x1 - z1)/2*tg(90-ang))/";
//        y += 20;
//        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

//        str = "     (1 + 1/2tg(90-ang))";
//        y += 20;
//        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

//        str = "90 - ang = " + QString::number(mParameter.angFi, 'f', 2);
//        y += 40;
//        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

//        str = "tan(90 - ang) = " + QString::number(mParameter.t, 'f', 2);
//        y += 20;
//        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "z3 = " + QString::number(mParameter.y3, 'f', 2);
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "x3 = " + QString::number(mParameter.x3, 'f', 2);
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());
    }
    else if(mIsCountTest) {
//        float y = 40;
//        for(auto str : mShowString) {
//            printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());
//            y += 20;
//        }
        str = "z - z1 = tg(90-ang) * (x - x1)";
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "z = tg(90-ang) * (x - x1) + z1";
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "z = tg(90- " + QString::number(mParameter.ang, 'f', 2) + ") * (x - " + QString::number(mParameter.x1, 'f', 2) + ") + " + QString::number(mParameter.y1, 'f', 2);
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "z = k * x + b";
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = "z = " + QString::number(mParameter.k, 'f', 2) + " * x + " + QString::number(mParameter.b, 'f', 2);
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());

        str = QString::number(mParameter.y2, 'f', 2) + " = " + QString::number(mParameter.k, 'f', 2) + " * " + QString::number(mParameter.x2, 'f', 2) + " + " + QString::number(mParameter.b, 'f', 2);
        y += 20;
        printString(10 * m_wSc, y * m_hSc, str.toStdString().c_str());
    }
}

void GlWidget::counting()
{
    float x1 = mParameter.x1, z1 = mParameter.y1, x2 = mParameter.x2, z2 = mParameter.y2, ang = mParameter.ang;
    float angFi = mParameter.angFi = 90 - ang;
    float angFiRad = angFi * PI / 180;
    float k = mParameter.k = mParameter.t = tan(angFiRad);
    float b = mParameter.b = mParameter.y1 - k * mParameter.x1;
    float k2 = mParameter.k2 = -1 / k;
    float b2 = mParameter.b2 = x2 / k + z2;

    mParameter.y3 = (b - k * b2 / k2) / (1 - k / k2);
    mParameter.x3 = (mParameter.y3 - b2) / k2;
//    mParameter.y3 = (x2 / k + z2 - (x1 * k - z1) / (2 * k)) / (1 + 1 / (2 * k));
//    mParameter.x3 = (mParameter.y3 + x1 * k - z1) / k;

    mIsCountPoint = true;
    mIsCountTest = false;
    update();
}

void GlWidget::countingTest()
{
    float angFi = mParameter.angFi = 90 - mParameter.ang;
    float angFiRad = angFi * PI / 180;
    float k = mParameter.k = mParameter.t = tan(angFiRad);
    float b = mParameter.b = mParameter.y1 - k * mParameter.x1;
    mParameter.y2 = k * mParameter.x2 + b;

    mIsCountPoint = false;
    mIsCountTest = true;
    update();
}
