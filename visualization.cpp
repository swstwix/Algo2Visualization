#include "visualization.h"
#include "ui_visualization.h"

Visualization::Visualization(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Visualization)
{
    ui->setupUi(this);
    this->setCentralWidget(new QWidget(this));
}

Visualization::~Visualization()
{
    delete ui;
}

void Visualization::on_action_4_triggered()
{
    QDialog* dialog = new InputDialog(this);
    dialog->setVisible(true);
}

void Visualization::on_action_triggered()
{
    QStringList filters;
    filters << "Text files (*.txt)";
    QFileDialog dialog(this);
    dialog.setNameFilters(filters);
    fileName=dialog.getOpenFileName();
    if (!fileName.isEmpty())
        startVisualize();
}

void Visualization::paintEvent(QPaintEvent * paintEvent){
    if (fileName.isNull())
    {
    }
    else
    {
        QPainter painter(this);
        DrawReshenie(painter, fileName);
        painter.end();
    }
}

job* jobs;
int n, m;

bool myfunc(job j1, job j2) {
    return j1.a1 < j2.a1;
}

bool myfunc2(job j1, job j2) {
    return (j1.a1+j2.a2) < (j2.a1+j2.a2);
}

bool myfunc3(job j1, job j2) {
    return (j1.a1+j2.a2+j2.a3) < (j2.a1+j2.a2+j2.a3);
}

bool myfunc4(job j1, job j2) {
    return (j2.a2+j2.a3) < (j2.a2+j2.a3);
}

int analyze() {
    int ans = 0;
    int cur_time = 0;
    int last_free = 0;
    queue<job> queue;
    priority_queue<work> working;
    for (int i = 0; i < n; i++)
        queue.push(jobs[i]);
    while (!queue.empty() || !working.empty())
    {
        if (working.size() < m && !queue.empty())
        {
            job j = queue.front();
            queue.pop();
            work work(j, max(last_free,j.a1)+j.a2, j.a3, working.size()+1);
            working.push(work);
            continue;
        }

        work t = working.top();
        working.pop();
        ans = max(ans, t.end + t.spend);
        last_free = t.end;
    }
    return ans;
}

void Visualization::DrawOneWork(QPainter& q, work w, int res){
    int s = 30;
    int by = 50+s;
    int ey = this->height()-s;
    double dy = (ey - by) / m;
    int bx = s;
    int ex = this->width() - s - 20;
    double dx = (ex - bx) / res;

    int x0= bx+(w.end-w.j.a2)*dx;
    int y0= by+(m-w.n-1)*dy;

    QString mes = "Деталь { " + QString::number(w.j.a1)+","+QString::number(w.j.a2)+", "+QString::number(w.j.a3)+" };";

    q.fillRect(x0+5,y0+5,w.j.a2*dx-10,dy-10, QColor(100+qrand()%150,100+qrand()%150,100+qrand()%150));
    q.drawText(x0+6,y0+14, mes);
}

void Visualization::DrawReshenie(QPainter& g, QString fileName){
    int s = 30;
    sort(jobs, jobs+n, myfunc);
    int res = analyze();
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QString message =  "Ответ на задачу : " + QString::number(res);
    g.drawText(s, 40, message);
    g.drawLine(s, 50, s, this->height() - s); // oy
    g.drawLine(s, 50, s-5, 60); // стрелочка
    g.drawLine(s, 50, s+5, 60);
    g.drawLine(s, this->height()-s, this->width() - s, this->height() - s); // ox
    g.drawLine(this->width() - s - 10, this->height() - s+5, this->width() - s, this->height() - s); // стрелочка
    g.drawLine(this->width() - s - 10, this->height() - s-5, this->width() - s, this->height() - s);
    for (int i = 0; i < m; i++)
    {
        int b = 50+s;
        int e = this->height()-s;
        double d = (e - b) / m;
        g.drawLine(s-5, b + i*d, s+5, b+i*d);
    }
    for (int i = 0; i < m; i++)
    {
        int b = 50+s;
        int e = this->height()-s;
        double d = (e - b) / m;
        g.drawText(s-10, b+i*d+d/2, QString::number(m-i));
    }
    for (int i = 0; i < res; i++)
    {
        int b = s;
        int e = this->width() - s - 20;
        double d = (e - b) / res;
        g.drawLine(b+(i+1)*d, this->height()-s - 5, b+(i+1)*d, this->height()-s+5);
    }
    for (int i = 0; i <= res; i++)
    {
        int b = s;
        int e = this->width() - s - 20;
        double d = (e - b) / res;
        g.drawText(b+i*d-3, this->height()-s+15, QString::number(i)   );
    }


    int ans = 0;
    int cur_time = 0;
    int last_free = 0;
    queue<job> que;
    queue<int> stanki;
    priority_queue<work> working;
    for (int i = 0; i < n; i++)
        que.push(jobs[i]);
    for (int i = 0; i < m; i++)
        stanki.push(i);
    while (!que.empty() || !working.empty())
    {
        if (working.size() < m && !que.empty())
        {
            job j = que.front();
            que.pop();
            work work(j, max(last_free,j.a1)+j.a2, j.a3, stanki.front());
            stanki.pop();
            working.push(work);
            continue;
        }

        work t = working.top();
        working.pop();
        stanki.push(t.n);
        DrawOneWork(g, t, res);
        ans = max(ans, t.end + t.spend);
        last_free = t.end;
    }
}

void Visualization::startVisualize(){
    ifstream in(fileName.toStdString().c_str());
    in >> n >> m;
    jobs = new job[n];
    for (int i = 0; i < n; i++)
        in >> jobs[i].a1 >> jobs[i].a2 >> jobs[i].a3;
    in.close();
    this->repaint();
}
