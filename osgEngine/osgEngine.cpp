/* OpenSceneGraph example, osgteapot.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <osg/Geode>
#include <osg/LineWidth>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/Quat>
#include <osg/ShapeDrawable>
#include <osg/TexGen>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgUtil/PrintVisitor>
#include <osgViewer/Viewer>
#include <osg/GraphicsContext>

// The classic OpenGL teapot... taken form glut-3.7/lib/glut/glut_teapot.c

/* Copyright (c) Mark J. Kilgard, 1994. */

/**
(c) Copyright 1993, Silicon Graphics, Inc.

ALL RIGHTS RESERVED

Permission to use, copy, modify, and distribute this software
for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies and that
both the copyright notice and this permission notice appear in
supporting documentation, and that the name of Silicon
Graphics, Inc. not be used in advertising or publicity
pertaining to distribution of the software without specific,
written prior permission.

THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU
"AS-IS" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR
OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN NO
EVENT SHALL SILICON GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE
ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL, INDIRECT OR
CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE,
SAVINGS OR REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR
NOT SILICON GRAPHICS, INC.  HAS BEEN ADVISED OF THE POSSIBILITY
OF SUCH LOSS, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
ARISING OUT OF OR IN CONNECTION WITH THE POSSESSION, USE OR
PERFORMANCE OF THIS SOFTWARE.

US Government Users Restricted Rights

Use, duplication, or disclosure by the Government is subject to
restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer
Software clause at DFARS 252.227-7013 and/or in similar or
successor clauses in the FAR or the DOD or NASA FAR
Supplement.  Unpublished-- rights reserved under the copyright
laws of the United States.  Contractor/manufacturer is Silicon
Graphics, Inc., 2011 N.  Shoreline Blvd., Mountain View, CA
94039-7311.

OpenGL(TM) is a trademark of Silicon Graphics, Inc.
*/

template <typename T>
std::string Vec3ToString(const T& vec) {
    std::stringstream ss;
    ss << "{" << vec.x() << "," << vec.y() << "," << vec.z() << "}";
    return ss.str();
}

class RotateCallback : public osg::NodeCallback {
   public:
    RotateCallback() : osg::NodeCallback(), enabled_(true) {}
    void operator()(osg::Node* node, osg::NodeVisitor* nv) {
        osg::MatrixTransform* xform = dynamic_cast<osg::MatrixTransform*>(node);
        if (xform && enabled_) {
            double t = nv->getFrameStamp()->getSimulationTime();
            xform->setMatrix(osg::Matrix::rotate(t, osg::Vec3(0, 1, 0)));
            auto mat = xform->getMatrix();
            // std::cout<<"Pos:"<<Vec3ToString(mat.getTrans())<<std::endl;
            // auto eular =  mat.getRotate()*osg::Vec3(0,0,-1);
            std::cout << "node:" << node->getName() << std::endl;
            auto eular = mat.getRotate().asVec3();
            std::cout << "Rot:" << Vec3ToString(eular) << std::endl;
        }
        traverse(node, nv);
    }

    bool enabled_;
};

// 创建带箭头的向量
osg::ref_ptr<osg::MatrixTransform> createVectorArrow(const osg::Vec3& start,
                                                     const osg::Vec3& direction,
                                                     float length,
                                                     const osg::Vec4& color) {
    osg::ref_ptr<osg::MatrixTransform> rootMat = new osg::MatrixTransform();

    // 计算终点
    osg::Vec3 end = start + direction * length;
    osg::Vec3 mid = start + direction * (length * 0.8f);  // 箭头开始位置

    // 1. 创建线段部分
    osg::ref_ptr<osg::Geode> lineGeode = new osg::Geode();
    osg::ref_ptr<osg::Geometry> lineGeometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> lineVertices = new osg::Vec3Array();
    lineVertices->push_back(start);
    lineVertices->push_back(mid);
    lineGeometry->setVertexArray(lineVertices.get());

    osg::ref_ptr<osg::Vec4Array> lineColors = new osg::Vec4Array();
    lineColors->push_back(color);
    lineGeometry->setColorArray(lineColors.get(), osg::Array::BIND_OVERALL);
    lineGeometry->addPrimitiveSet(
        new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));

    // 设置线宽
    osg::StateSet* lineSS = lineGeometry->getOrCreateStateSet();
    lineSS->setAttribute(new osg::LineWidth(2.0f));

    lineGeode->addDrawable(lineGeometry.get());
    rootMat->addChild(lineGeode.get());

    // 原点
    osg::ref_ptr<osg::Sphere> sphere =
        new osg::Sphere(osg::Vec3(0, 0, 0), length * 0.05f);
    osg::ref_ptr<osg::ShapeDrawable> sphereDrawable =
        new osg::ShapeDrawable(sphere);
    sphereDrawable->setColor(osg::Vec4(1, 0, 0, 1));

    osg::ref_ptr<osg::MatrixTransform> sphereTransform =
        new osg::MatrixTransform();
    sphereTransform->setMatrix(osg::Matrix::translate(start));
    sphereTransform->addChild(sphereDrawable.get());
    rootMat->addChild(sphereTransform.get());

    // 2. 创建箭头部分（圆锥）
    // 创建圆锥
    float arrowRadius = length * 0.05f;
    float arrowHeight = length * 0.2f;
    osg::ref_ptr<osg::Cone> cone =
        new osg::Cone(osg::Vec3(0, 0, 0), arrowRadius, arrowHeight);
    osg::ref_ptr<osg::ShapeDrawable> coneDrawable =
        new osg::ShapeDrawable(cone);
    coneDrawable->setColor(color);

    // 计算箭头旋转矩阵
    osg::ref_ptr<osg::MatrixTransform> arrowTransform =
        new osg::MatrixTransform();

    // 需要将圆锥旋转到方向向量的方向
    osg::Vec3 zAxis(0, 0, 1);  // 圆锥默认沿Z轴
    osg::Vec3 dir = direction;
    dir.normalize();

    // 计算旋转轴和角度
    osg::Vec3 axis = zAxis ^ dir;     // 叉积得到旋转轴
    float angle = acos(zAxis * dir);  // 点积得到角度

    osg::Matrix matrix;
    matrix.makeTranslate(mid);                     // 平移到箭头位置
    matrix.preMultRotate(osg::Quat(angle, axis));  // 旋转到方向

    arrowTransform->setMatrix(matrix);
    arrowTransform->addChild(coneDrawable.get());

    rootMat->addChild(arrowTransform.get());

    return rootMat;
}

// 使用示例：绘制法线向量
osg::ref_ptr<osg::MatrixTransform> createNormalVector(const osg::Vec3& position,
                                                      const osg::Vec3& normal,
                                                      float scale = 1.0f) {
    // 假设normal是单位向量，scale控制长度
    return createVectorArrow(position, normal, scale,
                             osg::Vec4(0, 1, 0, 1));  // 绿色法线
}

osg::ref_ptr<osg::MatrixTransform> createCube(osg::Vec3 Pos, osg::Vec3 color) {
    osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0, 0, 0), 0.2f);
    osg::ref_ptr<osg::ShapeDrawable> boxDrawable = new osg::ShapeDrawable(box);
    boxDrawable->setColor(osg::Vec4(color, 1));

    osg::ref_ptr<osg::MatrixTransform> cubeTransform =
        new osg::MatrixTransform();
    cubeTransform->setMatrix(osg::Matrix::translate(Pos));
    cubeTransform->addChild(boxDrawable.get());
    return cubeTransform;
}

osg::Group* createLine() {
    // 把 geode 放进一个新的 Group，从而确保返回的对象拥有并保持对 geode
    // 的引用。
    osg::Group* root = new osg::Group();

    osg::Vec3 ori = osg::Vec3(0, 0, 0);
    osg::Vec3 dir = osg::Vec3(1, 1, 1);
    dir.normalize();
    // 使用示例
    osg::ref_ptr<osg::MatrixTransform> line = createNormalVector(ori,  // 起点
                                                                 dir   // 方向
    );

    // line->addUpdateCallback(new RotateCallback());
    line->setMatrix(osg::Matrix::translate(osg::Vec3(0, 0, 0)));

    auto lineRotate = line->getMatrix().getRotate().asVec3();
    std::cout << "Line Rot:" << Vec3ToString(lineRotate) << std::endl;

    osg::ref_ptr<osg::MatrixTransform> coordOrigin =
        createCube(osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 0));
    osg::ref_ptr<osg::MatrixTransform> coordX =
        createCube(osg::Vec3(1, 0, 0), osg::Vec3(1, 0, 0));
    osg::ref_ptr<osg::MatrixTransform> coordY =
        createCube(osg::Vec3(0, 1, 0), osg::Vec3(0, 1, 0));
    osg::ref_ptr<osg::MatrixTransform> coordZ =
        createCube(osg::Vec3(0, 0, 1), osg::Vec3(0, 0, 1));
    root->addChild(coordOrigin.get());
    root->addChild(coordX.get());
    root->addChild(coordY.get());
    root->addChild(coordZ.get());

    root->addChild(line.get());
    root->setName("line_root");

    return root;
}

int main(int, char**) {
#if 1

    // create viewer on heap as a test, this looks to be causing problems
    // on init on some platforms, and seg fault on exit when multi-threading on
    // linux. Normal stack based version below works fine though...

    // construct the viewer.
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    osgDB::FilePathList& pathList =
        osgDB::Registry::instance()->getDataFilePathList();

    std::cout << "OSG Data File Paths:" << std::endl;
    for (osgDB::FilePathList::iterator it = pathList.begin();
         it != pathList.end(); ++it) {
        std::cout << "  " << *it << std::endl;
    }

    // add model to viewer.
    osg::Group* root = new osg::Group();
    root->addChild(createLine());

    viewer->setSceneData(root);

    // auto& wsi = osg::GraphicsContext::getWindowingSystemInterfaces();
    // std::cout << "WSI count = " << wsi.size() << std::endl;

    return viewer->run();

#else

    // construct the viewer.
    osgViewer::Viewer viewer;

    // create the windows and run the threads.
    return viewer.run();
#endif
}
