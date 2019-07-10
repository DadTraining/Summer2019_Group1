#include "HomeScene.h"

USING_NS_CC;

Size visibleSize;
Size tileMapSize;
Vec2 cameraDistance, buttonDistance;
float xMin, xMax, yMin, yMax;
bool a, b, c, d;

Scene* HomeScene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = HomeScene::create();

	scene->addChild(layer);

	return scene;
}

bool HomeScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	mainCharacter = new MainCharacter(this);
	visibleSize = Director::getInstance()->getVisibleSize();

	a = b = c = d = false;
	tileMap = TMXTiledMap::create("res/tiledMaps/home/home.tmx");
	tileMap->setPosition(Vec2(0, 0));
	tileMapSize = tileMap->getContentSize();
	addChild(tileMap, 0);

	auto tileMap1 = TMXTiledMap::create("res/tiledMaps/home/upperHome.tmx");
	tileMap1->setPosition(Vec2(0, 0));
	tileMapSize = tileMap1->getContentSize();
	addChild(tileMap1, 3);



	body = PhysicsBody::createBox(mainCharacter->GetSprite()->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	body->setDynamic(true);
	body->setGravityEnable(false);
	auto ob = tileMap->objectGroupNamed("mainCharacter");
	float xx = ob->getObject("mc")["x"].asFloat();
	float yy = ob->getObject("mc")["y"].asFloat();
	log("%f,%f", tileMapSize.height, yy);
	mainCharacter->GetSprite()->setPosition(xx,yy);
	mainCharacter->GetSprite()->setPhysicsBody(body);
	camera = Camera::create();
	camera->setPosition(mainCharacter->GetSprite()->getPosition());
	addChild(camera);
	frameButton = Sprite::create("res/buttons/frameButton.png");
	auto frameButtonSize = frameButton->getBoundingBox().size;
	frameButton->setPosition(Vec2(frameButtonSize.width, frameButtonSize.height)+camera->getPosition()-visibleSize/2);
	auto frameButtonPosition = frameButton->getPosition();
	frameButton->setVisible(false);
	addChild(frameButton, 8);

	upButton = ui::Button::create("res/buttons/upButtonNormal.png", "res/buttons/upButtonPressed.png");
	upButton->setAnchorPoint(Vec2(0.5, 1));
	upButton->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y + frameButtonSize.height / 2));
	addChild(upButton, 4);

	downButton = ui::Button::create("res/buttons/downButtonNormal.png", "res/buttons/downButtonPressed.png");
	downButton->setAnchorPoint(Vec2(0.5, 0));
	downButton->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y - frameButtonSize.height / 2));
	addChild(downButton, 5);

	leftButton = ui::Button::create("res/buttons/leftButtonNormal.png", "res/buttons/leftButtonPressed.png");
	leftButton->setAnchorPoint(Vec2(0, 0.5));
	leftButton->setPosition(Vec2(frameButtonPosition.x - frameButtonSize.width / 2, frameButtonPosition.y));
	addChild(leftButton, 6);

	rightButton = ui::Button::create("res/buttons/rightButtonNormal.png", "res/buttons/rightButtonPressed.png");
	rightButton->setAnchorPoint(Vec2(1, 0.5));
	rightButton->setPosition(Vec2(frameButtonPosition.x + frameButtonSize.width / 2, frameButtonPosition.y));
	addChild(rightButton, 7);

	body->setRotationEnable(false);


	cameraDistance = camera->getPosition() - mainCharacter->GetSprite()->getPosition();
	buttonDistance = frameButton->getPosition() - camera->getPosition();
	auto contactListener = EventListenerPhysicsContact::create();
	
	AddListener();



	auto s = tileMap->getLayer("obstacles");
	s->setVisible(false);
	Size layerSize = s->getLayerSize();
	for (int i = 0; i < layerSize.width; i++)
	{
		for (int j = 0; j < layerSize.height; j++)
		{
			auto tileSet = s->getTileAt(Vec2(i, j));
			if (tileSet != NULL)
			{
				auto physics = PhysicsBody::createBox(tileSet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
				physics->setDynamic(false);
				
				tileSet->setPhysicsBody(physics);
			}
		}
	}

	

	scheduleUpdate();

	return true;
}

void HomeScene::update(float deltaTime)
{
	if (a || b || c || d)
	{
		//OnButtonHold(deltaTime);

	}
	SetPositionButton();
	SetCamera(mainCharacter->GetSprite()->getPosition() + cameraDistance);
}

void HomeScene::AddListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HomeScene::OnTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HomeScene::OnTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HomeScene::OnTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	upButton->addTouchEventListener(CC_CALLBACK_2(HomeScene::UpButtonTouched, this));
	downButton->addTouchEventListener(CC_CALLBACK_2(HomeScene::DownButtonTouched, this));
	leftButton->addTouchEventListener(CC_CALLBACK_2(HomeScene::LeftButtonTouched, this));
	rightButton->addTouchEventListener(CC_CALLBACK_2(HomeScene::RightButtonTouched, this));

}

bool HomeScene::OnTouchBegan(Touch* touch, Event* event)
{
	return true;
}

bool HomeScene::OnTouchEnded(Touch* touch, Event* event)
{
	return true;
}

void HomeScene::OnTouchMoved(Touch* touch, Event* event)
{
	Vec2 newPosition = touch->getPreviousLocation()- touch->getLocation() + camera->getPosition();
	SetCamera(newPosition);
}

//void HomeScene::OnButtonHold(float deltaTime)
//{
//	if (std::find(heldButtons.begin(), heldButtons.end(), 1) != heldButtons.end())
//	{
//		body->setVelocity(Vec2(0, 100));
//	}
//	if (std::find(heldButtons.begin(), heldButtons.end(), 2) != heldButtons.end())
//	{
//		body->setVelocity(Vec2(0, -100));
//	}
//	if (std::find(heldButtons.begin(), heldButtons.end(), 3) != heldButtons.end())
//	{
//		body->setVelocity(Vec2(-100, 0));
//	}
//	if (std::find(heldButtons.begin(), heldButtons.end(), 4) != heldButtons.end())
//	{
//		body->setVelocity(Vec2(100, 0));
//	}
//	SetCamera(mainCharacter->GetSprite()->getPosition() + cameraDistance);
//}

void HomeScene::UpButtonTouched(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		//heldButtons.push_back(1);
		body->setVelocity(Vec2(0, 100));
		//mainCharacter->run();
		//mainCharacter->run();
		//SetCamera(mainCharacter->GetSprite()->getPosition() + cameraDistance);

	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//heldButtons.erase(std::remove(heldButtons.begin(), heldButtons.end(), 1), heldButtons.end());
		body->setVelocity(Vec2(0, 0));
		//mainCharacter->stop();
	}

}

void HomeScene::DownButtonTouched(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		//heldButtons.push_back(2);
		body->setVelocity(Vec2(0, -100));
		//mainCharacter->run();
		//SetCamera(mainCharacter->GetSprite()->getPosition() + cameraDistance);

	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//heldButtons.erase(std::remove(heldButtons.begin(), heldButtons.end(), 2), heldButtons.end());
		body->setVelocity(Vec2(0, 0));
		//mainCharacter->stop();

	}

}

void HomeScene::LeftButtonTouched(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		//heldButtons.push_back(3);
		body->setVelocity(Vec2(-100, 0));
		//mainCharacter->run();
		//SetCamera(mainCharacter->GetSprite()->getPosition() + cameraDistance);

		//c = true;
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//heldButtons.erase(std::remove(heldButtons.begin(), heldButtons.end(), 3), heldButtons.end());
		body->setVelocity(Vec2(0, 0));
		//mainCharacter->stop();

		//c = false;

	}

}

void HomeScene::RightButtonTouched(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		///heldButtons.push_back(4);
		body->setVelocity(Vec2(100, 0));
		//mainCharacter->run();
		//SetCamera(mainCharacter->GetSprite()->getPosition() + cameraDistance);

		//d = true;
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//heldButtons.erase(std::remove(heldButtons.begin(), heldButtons.end(), 4), heldButtons.end());
		body->setVelocity(Vec2(0, 0));
		//mainCharacter->stop();

		//d = false;
	}

}

void HomeScene::CreateFixturesLayer(TMXLayer* layer)
{
	Size layerSize = layer->getLayerSize();
	
}

void HomeScene::SetPositionButton()
{
	auto frameButtonSize = frameButton->getBoundingBox().size;
	auto frameButtonPosition = frameButton->getPosition();
	upButton->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y + frameButtonSize.height / 2));
	downButton->setPosition(Vec2(frameButtonPosition.x, frameButtonPosition.y - frameButtonSize.height / 2));
	leftButton->setPosition(Vec2(frameButtonPosition.x - frameButtonSize.width / 2, frameButtonPosition.y));
	rightButton->setPosition(Vec2(frameButtonPosition.x + frameButtonSize.width / 2, frameButtonPosition.y));
}

void HomeScene::SetCamera(Vec2 pos)
{
	float xMin = visibleSize.width / 2, xMax = tileMapSize.width - xMin, yMin = visibleSize.height / 2, yMax = tileMapSize.height - yMin;
	if (pos.x < xMin)
	{
		pos.x = xMin;
	}
	else if (pos.x > xMax)
	{
		pos.x = xMax;
	}
	if (pos.y < yMin)
	{
		pos.y = yMin;
	}
	else if (pos.y > yMax)
	{
		pos.y = yMax;
	}
	camera->setPosition(Vec2(pos.x, pos.y));
	frameButton->setPosition(pos + buttonDistance);
}

