#include "iscissor_tool.h"
#include <QBitmap>
#include <cmath>


void
IScissorTool:: init(QPixmap pm, float scale, QColor, QColor )
{
    pixmap = pm;
    image = pm.toImage();
    scaleFactor = scale;
    getGradientMap();
    is_start = 1;
    init_seed = 0;
    is_closed = 0;
}

void
IScissorTool:: onMousePress(QPoint pos)
{   
    int x = pos.x();
    int y = pos.y();
    if(is_start)
    {
        if (is_closed) {
            getMask(x, y);
        }
        if(!init_seed)  // First mouse click after loading image
        {
            seed_vector.clear();
            fullPath_vector.clear();
            init_seed = 1;
        }
        else {
            closeDetect(x, y);
            fullPath_vector.push_back(shortPath_vector);
        }
        if (! is_closed) {
            seed_x = x;
            seed_y = y;
            seed_vector.push_back(QPoint(x,y));
        }
        //qDebug() << "click" << x << y;
    }
}


void
IScissorTool:: onMouseMove(QPoint pos)
{
    int x = pos.x();
    int y = pos.y();
    if (init_seed==0 or is_closed==1) return; //Return if mouse is not clicked
    // Get bounding box of livewire
    int x1 = MIN(x, seed_x);
    int y1 = MIN(y, seed_y);
    int x2 = MAX(x, seed_x);
    int y2 = MAX(y, seed_y);

    initVector(x1,y1,x2,y2);

    priority_queue<Node> que;
    que.push(node_vector[seed_num]);
    int count =0;
    while (!que.empty())
    {
        count++;
        Node q = que.top();                 // This is node with minimum total cost
        que.pop();
        if (node_vector[q.num].state == EXPANDED)
            continue;
        q.state = EXPANDED;
        node_vector[q.num].state = EXPANDED;
        for (int i = -1 ;i <= 1;i++)         // Iterate y from y-1 to y+1
            for (int j = -1 ;j <= 1;j++)     // Iterate x from x-1 to x+1
            {
                if (i == 0 && j==0) // Skip if the point is q
                    continue;
                // Skip when neighbour is outside bounding box
                if (q.col+j < x1 || q.row+i < y1 || q.col+j > x2 || q.row+i > y2)
                    continue;
                // r is a neighbour of q
                Node r = node_vector[(q.row + i - y1) * (x2-x1+1) + (q.col + j -x1)];
                if (r.state == INITIAL)      // INITIAL means nothing has done yet
                {
                    r.prevNode = &q;
                    r.totalCost = q.totalCost + q.linkCost[(i+1)*3 + j+1];
                    r.state = ACTIVE;
                    r.prevNodeNum = q.num;
                    node_vector[r.num].prevNode = &q;
                    node_vector[r.num].prevNodeNum = q.num;
                    node_vector[r.num].totalCost = q.totalCost + q.linkCost[(i+1) * 3 + j + 1];
                    node_vector[r.num].state = ACTIVE;
                    que.push(r);            // Insert r into the que
                }
                else if (r.state == ACTIVE)  // ACTIVE means r is already in que
                {
                    if (q.totalCost + q.linkCost[(i+1)*3 + j+1] < r.totalCost)
                    {
                        r.prevNode = &q;
                        r.totalCost = q.totalCost + q.linkCost[(i+1) * 3 + j + 1];
                        r.prevNodeNum = q.num;
                        node_vector[r.num].prevNode = &q;
                        node_vector[r.num].prevNodeNum = q.num;
                        node_vector[r.num].totalCost = q.totalCost + q.linkCost[(i+1) * 3 + j + 1];
                        que.push(r);    //update que
                    }
                }
            }
    }

    Node des_node = node_vector[(y-y1) * (x2-x1+1) + (x-x1)];
    QImage line_image = image.copy();
    QPen blue_pen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPainter painter(&line_image);
    painter.setPen(blue_pen);
    // Draw fixed lines
    for (int i=0; i<(int)fullPath_vector.size();i++) {
        for (int j=1;j<(int)fullPath_vector[i].size();j++) {    // for each shortPath_vector
            QPoint pt1 = fullPath_vector[i][j-1];
            QPoint pt2 = fullPath_vector[i][j];
            painter.drawLine(pt1,pt2);
        }
    }
    // Draw seed points
    painter.setPen(Qt::black);
    for (int i=0;i<(int)seed_vector.size();i++) {    // for each seeds
        QPoint pt = seed_vector[i];
        painter.drawEllipse(pt.x()-3, pt.y()-3, 6,6);
    }
    // Draw movable last line (livewire)
    QPen pen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    shortPath_vector.clear();
    shortPath_vector.push_back(QPoint(des_node.col,des_node.row));
    while (des_node.prevNodeNum != seed_num)
    {
        QPoint pointa(des_node.col, des_node.row);
        int next_num = des_node.prevNodeNum;
        des_node = node_vector[next_num];
        QPoint pointb(des_node.col,des_node.row);
        painter.drawLine(pointa, pointb);
        shortPath_vector.push_back(pointb);
    }
    shortPath_vector.push_back(QPoint(seed_x,seed_y));
    painter.end();
    emit canvasUpdated(QPixmap::fromImage(line_image));
}

void
IScissorTool:: initVector(int x1, int y1, int x2, int y2)
{
    node_vector.clear();
    int node_num = 0;
    for (int i = y1 ;i <= y2 ; i++) {
        for (int j = x1; j <= x2 ; j++)
        {
            struct Node node;
            node.row = i;   // y
            node.col = j;   // x
            node.state = INITIAL;
            node.totalCost = INF;
            node.prevNode = NULL;
            node.prevNodeNum = 0;
            node.num = node_num;
            int y = (i-1) * 3 + 1;
            int x = (j-1) * 3 + 1;
            int cnt = 0;
            // When pixel is at image boundary, link cost of neighbours is set to INF
            if (i == 0 || j == 0 || i == image.height() - 1 || j ==image.width() - 1)
            {
                for (int k = 0 ;k < 9;k++)
                    node.linkCost[k] = INF;
            }
            else
                for (int l = -1; l <= 1; l++)
                    for (int m = -1; m <= 1;m++)
                    {
                        if (l == 0 && m ==0)
                            node.linkCost[cnt]=0;
                        else
                            //node.linkCost[cnt] = grad_image.pixel(x+m,y+l); // slower, hence alternative is used
                            node.linkCost[cnt] = qRed(((QRgb*)grad_image.constScanLine(y+l))[x+m]); // faster
                        cnt++;
                    }

            node_vector.push_back(node);
            node_num++;
        }
    }
    seed_num = (seed_y - y1) * (x2-x1+1) + (seed_x - x1);
    node_vector[seed_num].totalCost = 0;
    node_vector[seed_num].prevNodeNum = seed_num;
    //qDebug() << "seed num" << seed_num << "vector size" << node_num;
}

void
IScissorTool:: closeDetect(int x, int y)
{
    bool closed = false;
    int distance, point_x, point_y;
    for (int i=0; i<(int)seed_vector.size(); ++i) {
        distance = sqrt(pow(seed_vector[i].x()-x, 2) + pow(seed_vector[i].y()-y, 2));
        if (distance < CLOSETHRES) {
            point_x = seed_vector[i].x();
            point_y = seed_vector[i].y();
            closed = true;
            break;
        }
    }
    if (closed) {
        onMouseMove(QPoint(point_x, point_y));
        is_closed = true;
        //statusbar->showMessage("Click inside boundary to get Image", 5000);
    }
}

void
IScissorTool:: getMask(int x, int y)
{
    QImage mask(image.width(), image.height(), QImage::Format_RGB888);
    mask.fill(Qt::white);
    for (int i=0; i<(int)fullPath_vector.size();i++) {
        for (int j=0;j<(int)fullPath_vector[i].size();j++) {    // for each shortPath_vector
            QPoint pt = fullPath_vector[i][j];
            mask.setPixel(pt, Qt::black);
        }
    }
    floodfill(mask, x, y, Qt::white, Qt::black);
    // Erase the boundary line by putting white points
    for (int i=0; i<(int)fullPath_vector.size();i++) {
        for (int j=0;j<(int)fullPath_vector[i].size();j++) {
            QPoint pt = fullPath_vector[i][j];
            mask.setPixel(pt, Qt::white);
        }
    }
    QPixmap pm = QPixmap::fromImage(image);
    pm.setMask(QBitmap::fromImage(mask));
    emit canvasUpdated(pm);
}



// Store link costs as pixel color
void
IScissorTool:: getGradientMap()
{
    QImage tmp_img((image.width() - 2 ) * 3, (image.height() - 2) * 3, QImage::Format_ARGB32 );
    tmp_img.fill(Qt::white);
    double maxD = 0;
    double link = 0;
    QRgb pxl0, pxl1, pxl2, pxl3, pxl4, pxl5, pxl6, pxl7;
    QImage img = image;

    for (int i = 1 ;i < image.height() - 1; i++) {
        QRgb *row0 = (QRgb*)img.constScanLine(i-1);
        QRgb *row1 = (QRgb*)img.constScanLine(i);
        QRgb *row2 = (QRgb*)img.constScanLine(i+1);
        int y = (i-1) * 3 + 1;
        for (int j = 1; j < image.width() - 1; j++)
        {
            int x = (j-1) * 3 + 1;

            pxl0 = row1[ j+1];       //  Neighbours are oriented in this pattern
            pxl1 = row0[ j+1];     //  _____________
            pxl2 = row0[ j];       //  | 3 | 2 | 1 |
            pxl3 = row0[ j-1];     //  | 4 |   | 0 |
            pxl4 = row1[ j-1];     //  | 5 | 6 | 7 |
            pxl5 = row2[ j-1];     //  -------------
            pxl6 = row2[ j];
            pxl7 = row2[ j+1];

            //x-1,y-1
            double sum = 0;
            sum += pow( qRed(pxl4) - qRed(pxl2), 2);
            sum += pow( qGreen(pxl4) - qGreen(pxl2), 2);
            sum += pow( qBlue(pxl4) - qBlue(pxl2), 2);
            link = sqrt(sum / 6.0);
            if (link > maxD)
                maxD = link;
            ((QRgb*)tmp_img.scanLine(y-1))[x-1] = qRgb(link, link, link);

            //x-1,y+1
            sum = 0;
            sum += pow( qRed(pxl4) - qRed(pxl6), 2);
            sum += pow( qGreen(pxl4) - qGreen(pxl6), 2);
            sum += pow( qBlue(pxl4) - qBlue(pxl6), 2);

            link = sqrt(sum / 6.0);
            if (link > maxD)
                maxD = link;
            ((QRgb*)tmp_img.scanLine(y+1))[x-1] = qRgb(link, link, link);

            //x+1,y-1
            sum = 0;
            sum += pow( qRed(pxl0) - qRed(pxl2), 2);
            sum += pow( qGreen(pxl0) - qGreen(pxl2), 2);
            sum += pow( qBlue(pxl0) - qBlue(pxl2), 2);

            link = sqrt(sum / 6.0);
            if (link > maxD)
                maxD = link;
            ((QRgb*)tmp_img.scanLine(y-1))[x+1] = qRgb(link, link, link);

            //x+1,y+1
            sum = 0;
            sum += pow( qRed(pxl0) - qRed(pxl6), 2);
            sum += pow( qGreen(pxl0) - qGreen(pxl6), 2);
            sum += pow( qBlue(pxl0) - qBlue(pxl6), 2);

            link = sqrt(sum / 6.0);
            if (link > maxD)
                maxD = link;
            ((QRgb*)tmp_img.scanLine(y+1))[x+1] = qRgb(link, link, link);

            //x-1,y
            sum = 0;
            sum += pow( (qRed(pxl2) + qRed(pxl3) - qRed(pxl5) - qRed(pxl6))/4.0, 2);
            sum += pow( (qGreen(pxl2) + qGreen(pxl3) - qGreen(pxl5) - qGreen(pxl6))/4.0, 2);
            sum += pow( (qBlue(pxl2) + qBlue(pxl3) - qBlue(pxl5) - qBlue(pxl6))/4.0, 2);

            link = sqrt(sum / 3.0);
            if (link > maxD)
                maxD = link;
            ((QRgb*)tmp_img.scanLine(y))[x-1] = qRgb(link, link, link);

            //x,y-1
            sum = 0;
            sum += pow( (qRed(pxl4) + qRed(pxl3) - qRed(pxl0) - qRed(pxl1))/4.0, 2);
            sum += pow( (qGreen(pxl4) + qGreen(pxl3) - qGreen(pxl0) - qGreen(pxl1))/4.0, 2);
            sum += pow( (qBlue(pxl4) + qBlue(pxl3) - qBlue(pxl0) - qBlue(pxl1))/4.0, 2);

            link = sqrt(sum / 3.0);
            if (link > maxD)
                maxD = link;
            ((QRgb*)tmp_img.scanLine(y-1))[x] = qRgb(link, link, link);

            //x,y+1
            sum = 0;
            sum += pow( (qRed(pxl4) + qRed(pxl5) - qRed(pxl0) - qRed(pxl7))/4.0, 2);
            sum += pow( (qGreen(pxl4) + qGreen(pxl5) - qGreen(pxl0) - qGreen(pxl7))/4.0, 2);
            sum += pow( (qBlue(pxl4) + qBlue(pxl5) - qBlue(pxl0) - qBlue(pxl7))/4.0, 2);

            link = sqrt(sum / 3.0);
            if (link > maxD)
                maxD = link;
            ((QRgb*)tmp_img.scanLine(y+1))[x] = qRgb(link, link, link);

            //x+1,y
            sum = 0;
            sum += pow( (qRed(pxl2) + qRed(pxl1) - qRed(pxl7) - qRed(pxl6))/4.0, 2);
            sum += pow( (qGreen(pxl2) + qGreen(pxl1) - qGreen(pxl7) - qGreen(pxl6))/4.0, 2);
            sum += pow( (qBlue(pxl2) + qBlue(pxl1) - qBlue(pxl7) - qBlue(pxl6))/4.0, 2);

            link = sqrt(sum / 3.0);
            if (link > maxD)
                maxD = link;
            ((QRgb*)tmp_img.scanLine(y))[x+1] = qRgb(link, link, link);

        }
    }

    //update cost
    for (int i = 1 ;i < image.height() - 1; i++) {
        int y = (i-1) * 3 + 1;
        QRgb* row0 = (QRgb*)tmp_img.constScanLine(y-1);
        QRgb* row1 = (QRgb*)tmp_img.constScanLine(y);
        QRgb* row2 = (QRgb*)tmp_img.constScanLine(y+1);
        for (int j = 1; j < image.width() - 1; j++)
        {
            int x = (j-1) * 3 + 1;
            int clr;

            clr = (maxD - qRed(row0[x-1])) * 1.414;
            ((QRgb*)tmp_img.scanLine(y-1))[x-1] = qRgb(clr,clr,clr);

            clr = (maxD - qRed(row0[x+1])) * 1.414;
            ((QRgb*)tmp_img.scanLine(y-1))[x+1] = qRgb(clr,clr,clr);

            clr = (maxD - qRed(row2[x-1])) * 1.414;
            ((QRgb*)tmp_img.scanLine(y+1))[x-1] = qRgb(clr,clr,clr);

            clr = (maxD - qRed(row2[x+1])) * 1.414;
            ((QRgb*)tmp_img.scanLine(y+1))[x+1] = qRgb(clr,clr,clr);

            clr = (maxD - qRed(row0[x]));
            ((QRgb*)tmp_img.scanLine(y-1))[x] = qRgb(clr,clr,clr);

            clr = (maxD - qRed(row1[x-1]));
            ((QRgb*)tmp_img.scanLine(y))[x-1] = qRgb(clr,clr,clr);

            clr = (maxD - qRed(row1[x+1]));
            ((QRgb*)tmp_img.scanLine(y))[x+1] = qRgb(clr,clr,clr);

            clr = (maxD - qRed(row2[x]));
            ((QRgb*)tmp_img.scanLine(y+1))[x] = qRgb(clr,clr,clr);

        }
    }

    grad_image = tmp_img;
}

