#include "osg_viewer.h"

osg_viewer::osg_viewer()
{
}

osg_viewer::~osg_viewer()
{
}

void osg_viewer::add_point_cloud(std::vector<point_3d> & points)
{
	// �½�һ�� group
	osg::ref_ptr<osg::Group> root = new osg::Group();
	// ����һ�� viewer
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	// ���״̬�¼�
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));

	// �½�һ�� osg::GraphicsContext::Traits���������ڵ�����
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 40;
	traits->y = 40;
	traits->width = 600;
	traits->height = 480;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(gc.get());
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	GLenum buffer = (traits->doubleBuffer) ? GL_BACK : GL_FRONT;
	camera->setDrawBuffer(buffer);
	camera->setReadBuffer(buffer);

	// add this slave camera to the viewer, with a shift left of the projection matrix
	viewer->addSlave(camera.get());

	// ������������
	osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array();
	// ������ɫ
	osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array();

	for (size_t i = 0; i < points.size(); i++)
	{
		coords->push_back(osg::Vec3(points[i].x, points[i].y, points[i].z));

		color->push_back(osg::Vec4(points[i].r, points[i].g, points[i].b, 1.0f));
	}

	//����������
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	// ���ö�������
	geometry->setVertexArray(coords.get());
	geometry->setColorArray(color.get());
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::Vec3Array *normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));
	// geometry->setNormalArray(normals);
	// geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
	// ���ù�����ʽ
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, (int)points.size()));

	// ��ӵ�Ҷ�ڵ�
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(geometry.get());
	root->addChild(geode.get());

	//root->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF | StateAttribute::OVERRIDE);

	// �Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setSceneData(root.get());

	// ���ڴ�С�仯�¼�
	viewer->addEventHandler(new osgViewer::WindowSizeHandler);

	viewer->realize();
	viewer->run();
	//return viewer->run();
}
