#ifndef DUMPLING_TEST_H
#define DUMPLING_TEST_H

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

class Dumpling : public Test
{
public:
    Dumpling()
    {
        forceOn = false;
        torqueOn = false;

        //setup a dynamic body
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;

        //prepare a shape definition
        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(1, 1);

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &polygonShape;
        myFixtureDef.density = 1;

        //add four square shaped fixtures around the body center
        for (int i = 0; i < 3; i++) {
            myBodyDef.position.Set(-10 + i * 10, 20);
            bodies[i] = m_world->CreateBody(&myBodyDef);
            bodies[i]->CreateFixture(&myFixtureDef);
        }

        //static edge line
        myBodyDef.type = b2_staticBody;
        myBodyDef.position.Set(0, 0);

        b2EdgeShape edgeShape;
        edgeShape.Set(b2Vec2(-15, 0), b2Vec2(15, 0));
        myFixtureDef.shape = &edgeShape;
        //myFixtureDef.restitution = 1;

        floorBody = m_world->CreateBody(&myBodyDef);
        floorBody->CreateFixture(&myFixtureDef);
    }

    void Step(Settings* settings)
    {
        //run default physics and rendering
        Test::Step(settings);

        if (forceOn) {
            bodies[0]->ApplyForce(b2Vec2(0, 50), bodies[0]->GetWorldCenter(), true);
        }

        if (torqueOn) {
            bodies[0]->ApplyTorque(50, true);
        }
    }

    void Keyboard(unsigned char key)
    {
        switch(key)
        {
        case 'q':
            //apply gradual force upwards
            forceOn = !forceOn;
            break;
        case 'w':
            //apply immediate force upwards
            bodies[1]->ApplyLinearImpulse(b2Vec2(0, 50), bodies[1]->GetWorldCenter(), true);
            break;
        case 'e':
            bodies[2]->SetTransform(b2Vec2(10, 20), 0);
            break;
        case 'a':
            //apply gradual torque counter clockwise
            torqueOn = !torqueOn;
            break;
        case 's':
            //apply immediate spin counter clockwise
            bodies[1]->ApplyAngularImpulse(20, true);
            break;
        default:
            //run default behaviour
            Test::Keyboard(key);
        }
    }

    static Test* Create()
    {
        return new Dumpling();
    }

protected:
    b2Body* bodies[3];
    b2Body* floorBody;

    bool forceOn;
    bool torqueOn;
};

#endif