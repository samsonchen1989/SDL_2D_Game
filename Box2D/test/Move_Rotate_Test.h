#ifndef DUMPLING_TEST_H
#define DUMPLING_TEST_H

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

enum _moveState
{
    MS_STOP,
    MS_LEFT,
    MS_RIGHT,
};

class Dumpling : public Test
{
public:
    Dumpling()
    {
        clickedPoint = b2Vec2(0, 30);

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;

        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(1, 1);

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &polygonShape;
        myFixtureDef.density = 1;

        myBodyDef.position.Set(-10, 5);
        body = m_world->CreateBody(&myBodyDef);
        body->CreateFixture(&myFixtureDef);
        
        //a static body
        myBodyDef.type = b2_staticBody;
        myBodyDef.position.Set(0, 0);
        b2Body* staticBody = m_world->CreateBody(&myBodyDef);

        //add four walls to the static body
        polygonShape.SetAsBox(20, 1, b2Vec2(0, 0), 0);
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox(20, 1, b2Vec2(0,40), 0);
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox(1, 20, b2Vec2(-20, 20), 0);
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox(1, 20, b2Vec2(20, 20), 0);
        staticBody->CreateFixture(&myFixtureDef);

        myBodyDef.type = b2_dynamicBody;
        myBodyDef.position.Set(0, 18);

        b2Vec2 vertices[6];
        for (int i = 0; i < 6; i++) {
            float angle = -i/6.0 * 360 * DEGTORAD;
            vertices[i].Set(sinf(angle), cosf(angle));
        }
        vertices[0].Set(0, 4);
        polygonShape.Set(vertices, 6);

        myFixtureDef.shape = &polygonShape;
        clock = m_world->CreateBody(&myBodyDef);
        clock->CreateFixture(&myFixtureDef);
        clock->SetAngularVelocity(0);

        //zero gravity
        m_world->SetGravity(b2Vec2(0, 0));

        moveState = MS_STOP;
    }

    void Step(Settings* settings)
    {
        //run default physics and rendering
        Test::Step(settings);

        b2Vec2 vel = body->GetLinearVelocity();

        switch (moveState)
        {
        case MS_LEFT:
            vel.x = b2Max(vel.x - 0.1f, -5.0f);
            break;
        case MS_STOP:
            vel.x *= 0.98;
            break;
        case MS_RIGHT:
            vel.x = b2Min(vel.x + 0.1f, 5.0f);
            break;
        }

        body->SetLinearVelocity(vel);

        //draw point
        glPointSize(4);
        glBegin(GL_POINTS);
        glVertex2f(clickedPoint.x, clickedPoint.y);
        glEnd();

        float bodyAngle = clock->GetAngle();
        b2Vec2 toTarget = clickedPoint - clock->GetPosition();
        float desiredAngle = atan2f(-toTarget.x, toTarget.y);
        
        float nextAngle = bodyAngle + clock->GetAngularVelocity() / 2.5;
        float totalRotation = desiredAngle - nextAngle;
        while ( totalRotation < -180 * DEGTORAD ) totalRotation += 360 * DEGTORAD;
        while ( totalRotation >  180 * DEGTORAD ) totalRotation -= 360 * DEGTORAD;
        float desiredAngularVelocity = totalRotation * 60;
        float change = 1 * DEGTORAD; //allow 1 degree rotation per time step
        desiredAngularVelocity = b2Min(change, b2Max(-change, desiredAngularVelocity));
        float impulse = clock->GetInertia() * desiredAngularVelocity;
        clock->ApplyAngularImpulse(impulse, true);
    }

    void Keyboard(unsigned char key)
    {
        switch(key)
        {
        case 'q':
            moveState = MS_LEFT;
            break;
        case 'w':
            moveState = MS_STOP;
            break;
        case 'e':
            moveState = MS_RIGHT;
            break;
        default:
            //run default behaviour
            Test::Keyboard(key);
        }
    }

    void MouseDown(const b2Vec2& p)
    {
        clickedPoint = p;

        Test::MouseDown(p);
    }

    static Test* Create()
    {
        return new Dumpling();
    }

protected:
    b2Body* body;
    b2Body* clock;
    b2Vec2 clickedPoint;

    _moveState moveState;
};

#endif