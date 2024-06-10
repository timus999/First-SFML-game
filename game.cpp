#include "game.h"
#include "utility.h"
#include <iostream>
#include <cmath>

#define PI 3.141592625

Game::Game(const std::string &config)
{
  init(config);
}

void Game::init(const std::string &config)
{

  // set up default window parameters
  m_window.create(sf::VideoMode(1280, 720), "Assignment2");
  m_window.setFramerateLimit(60);

  spawnPlayer();
}

void Game::sMovement()
{

  // player movement logic is written here
  m_player->cTransform->velocity = {0, 0};
  if (m_player->cInput->up)
  {
    m_player->cTransform->velocity.y = -5;
  }
  if (m_player->cInput->right)
  {
    m_player->cTransform->velocity.x = 5;
  }
  if (m_player->cInput->left)
  {
    m_player->cTransform->velocity.x = -5;
  }
  if (m_player->cInput->down)
  {
    m_player->cTransform->velocity.y = 5;
  }

  for (auto e : m_entities.getEntities())
  {
    e->cTransform->pos.x += e->cTransform->velocity.x;
    e->cTransform->pos.y += e->cTransform->velocity.y;
  }
}

void Game::sUserInput()
{

  sf::Event event;

  while (m_window.pollEvent(event))
  {

    if (event.type == sf::Event::Closed)
    {
      m_running = false;
    }

    if (event.type == sf::Event::KeyPressed)
    {
      switch (event.key.code)
      {
      case sf::Keyboard::W:
        m_player->cInput->up = true;
        break;
      case sf::Keyboard::D:
        m_player->cInput->right = true;
        break;
      case sf::Keyboard::A:
        m_player->cInput->left = true;
        break;
      case sf::Keyboard::S:
        m_player->cInput->down = true;
        break;
      default:
        break;
      }
    }
    if (event.type == sf::Event::KeyReleased)
    {
      switch (event.key.code)
      {
      case sf::Keyboard::W:
        m_player->cInput->up = false;
        break;
      case sf::Keyboard::D:
        m_player->cInput->right = false;
        break;
      case sf::Keyboard::A:
        m_player->cInput->left = false;
        break;
      case sf::Keyboard::S:
        m_player->cInput->down = false;
        break;
      case sf::Keyboard::Escape:
        setPaused(m_paused);
        break;
      default:
        break;
      }
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
      switch (event.mouseButton.button)
      {
      case sf::Mouse::Left:
        spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
        break;
      case sf::Mouse::Right:
        spawnSpecialWeapon(m_player);
        break;
      default:
        break;
      }
    }
  }
}

void Game::sLifeSpan()
{

  for (auto &e : m_entities.getEntities())
  {
    if (e->cLifespan && e->cLifespan->remaining-- <= 0)
    {
      e->cLifespan->remaining = 0;
      e->destroy();
    }
  }
}

void Game::sCollision()
{

  // player and window collision detection

  float player_x = m_player->cTransform->pos.x;
  float player_y = m_player->cTransform->pos.y;
  float r = m_player->cCollision->radius;

  float ww = m_window.getSize().x;
  float wh = m_window.getSize().y;

  if (player_x + r > ww)
  {
    m_player->cTransform->pos.x = ww - r;
  }
  else if (player_x - r < 0)
  {
    m_player->cTransform->pos.x = r;
  }
  else if (player_y + r > wh)
  {
    m_player->cTransform->pos.y = wh - r;
  }
  else if (player_y - r <= 0)
  {
    m_player->cTransform->pos.y = r;
  }

  // player and enemy collision detection

  for (auto &e : m_entities.getEntities("enemy"))
  {
    if (m_player->cTransform->pos.distance(e->cTransform->pos) < r + e->cCollision->radius)
    {
      m_player->destroy();
      e->destroy();
      spawnPlayer();

      m_score = 0;
      return;
    }
  }

  // player and small enemy collision detection

  for (auto &e : m_entities.getEntities("smallEnemy"))
  {
    if (m_player->cTransform->pos.distance(e->cTransform->pos) < r + e->cCollision->radius)
    {
      m_player->destroy();
      e->destroy();
      spawnPlayer();
      m_score = 0;
      return;
    }
  }

  // bullet and enemy collision detection
  for (auto &bullet : m_entities.getEntities("bullet"))
  {

    for (auto &enemy : m_entities.getEntities("enemy"))
    {

      const auto &bulletPos = bullet->cTransform->pos;
      const auto &bulletRadius = bullet->cCollision->radius;
      const auto &enemyPos = enemy->cTransform->pos;
      const auto &enemyRadius = enemy->cCollision->radius;

      if (bulletPos.distance(enemyPos) < bulletRadius + enemyRadius)
      {
        bullet->destroy();
        enemy->destroy();
        spawnSmallEnemies(enemy);
        m_score += enemy->cScore->score;

      }
    }
  }

  // bullet and small enemy collision detection
  for (auto &bullet : m_entities.getEntities("bullet"))
  {

    for (auto &enemy_s : m_entities.getEntities("smallEnemy"))
    {

      const auto &bulletPos = bullet->cTransform->pos;
      const auto &bulletRadius = bullet->cCollision->radius;
      const auto &enemyPos = enemy_s->cTransform->pos;
      const auto &enemyRadius = enemy_s->cCollision->radius;

      if (bulletPos.distance(enemyPos) < bulletRadius + enemyRadius)
      {
        bullet->destroy();
        enemy_s->destroy();
        m_score += enemy_s->cScore->score;
      }
    }
  }
}

void Game::sEnemySpawner()
{

  // spawn enemy if the current frame - lastenenyspawn = 200
  if ((m_currentFrame - m_lastEnemySpawnTime) == 200)
    spawnEnemy();
}
void Game ::run()
{

  while (m_running)
  {


    if (!m_paused)
    {

    m_entities.update();
      sEnemySpawner();
      sMovement();
      sCollision();
      sLifeSpan();
    m_currentFrame = (m_currentFrame + 1);
    }
      sUserInput();
      sRender();

  }
}

void Game::setPaused(bool paused)
{
  m_paused = !m_paused;
}

void Game::spawnPlayer()
{
  // we create every entity by calling entitymanager.addEntity(tag))
  // this returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
  //
  auto entity = m_entities.addEntity("player");
  //
  // //give this entity a Transform so it spawns at (200, 200) with velocity (1, 1) and angle 0
  entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), 0.0f);
  //
  //     //this entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
  entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
  //
  //     //add an input component to the player so that we can use inputs
  entity->cInput = std::make_shared<CInput>();

  entity->cCollision = std::make_shared<CCollision>(32);
  //
  //     //since we want this entity to be our player, set our game's player variable to this entity
  //     //this goes slightly against the entitymanager paradigm, but we use the player so much it's worth it
  m_player = entity;
}

void Game::spawnEnemy()
{

  auto entity = m_entities.addEntity("enemy");

  entity->cCollision = std::make_shared<CCollision>(32.0f);
  float ex = randInRange(32, m_window.getSize().x - 32);
  float ey = randInRange(32, m_window.getSize().y - 32);

  float vertices = randInRange(3, 12);
  float speed = randInRange(2, 4);

  Vec2 pos = {ex, ey};

  Vec2 vel = m_player->cTransform->pos - pos;
  vel.normalize();
  vel *= speed;

  // rand color
  int r = randInRange(0, 255);
  int g = randInRange(0, 255);
  int b = randInRange(0, 255);

  entity->cTransform = std::make_shared<CTransform>(pos, vel, 0.0f);
  entity->cShape = std::make_shared<CShape>(32.0f, vertices, sf::Color(r, g, b),
                                            sf::Color(255, 255, 255), 4.0f);
  entity->cLifespan = std::make_shared<CLifespan>(300);

  entity->cScore = std::make_shared<CScore>(10);
  // record when last enemy was spawned

  m_lastEnemySpawnTime = m_currentFrame;
}

void Game ::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
  int vertices = e->cShape->circle.getPointCount();
  int step = 360 / vertices;

  for (int i = 0; i < vertices; i++)
  {
    auto entity = m_entities.addEntity("smallEnemy");

    float xVel = cos(i * step * PI / 180);
    float yVel = sin(i * step * PI / 180);
    Vec2 vel(xVel, yVel);
    auto speed = e->cTransform->velocity.length();
    vel *= speed;

    entity->cTransform = std::make_shared<CTransform>(e->cTransform->pos, vel, e->cTransform->angle);
    entity->cShape = std::make_shared<CShape>(16.0f, vertices, e->cShape->circle.getFillColor(),
                                              e->cShape->circle.getOutlineColor(), 2.0f);

    entity->cLifespan = std::make_shared<CLifespan>(50);
    entity->cCollision = std::make_shared<CCollision>(8);
     entity->cScore = std::make_shared<CScore>(5);
  }
}

void Game ::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mousePos)
{

  auto bullet = m_entities.addEntity("bullet");

  // normalize vector
  Vec2 vel = mousePos - entity->cTransform->pos;
  vel.normalize();
  vel *= 6;
  bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, vel, 0.0f);
  bullet->cShape = std::make_shared<CShape>(10, 8, sf::Color(255, 255, 0), sf::Color(255, 255, 255), 2);

  bullet->cCollision = std::make_shared<CCollision>(10);
  bullet->cLifespan = std::make_shared<CLifespan>(80);
}

void Game ::spawnSpecialWeapon(std::shared_ptr<Entity> e)
{
  int vertices = e->cShape->circle.getPointCount();
  int step = 360 / vertices;

  for (int i = 0; i < vertices; i++)
  {
    auto entity = m_entities.addEntity("bullet");

    float xVel = cos(i * step * PI / 180);
    float yVel = sin(i * step * PI / 180);
    Vec2 vel(xVel, yVel);
    vel *= 6;

    entity->cTransform = std::make_shared<CTransform>(e->cTransform->pos, vel, e->cTransform->angle);
    entity->cShape = std::make_shared<CShape>(12.0f, vertices, sf::Color(255, 255, 0),
                                              sf::Color(255, 255, 255), 2.0f);

    entity->cLifespan = std::make_shared<CLifespan>(50);
    entity->cCollision = std::make_shared<CCollision>(80);
  }
}

void Game::sRender()
{

  m_window.clear();

  for (auto e : m_entities.getEntities())
  {

    // set the position of the shape based on the entity's transform->pos
    e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

    // set the rotation of the shape based  on the entity's transform angle
    e->cTransform->angle += 1.0f;
    e->cShape->circle.setRotation(m_player->cTransform->angle);

    m_window.draw(e->cShape->circle);

    // set opacity proportional to it's remaining lifespan
    if (e->cLifespan)
    {
      int opacity = 255 * (e->cLifespan->remaining) /
                    (e->cLifespan->total);
      // Fill color
      const sf::Color &currFill =
          e->cShape->circle.getFillColor();
      sf::Color fillColor(currFill.r, currFill.g, currFill.b,
                          opacity);
      e->cShape->circle.setFillColor(fillColor);

      // Outline color
      const sf::Color &currOutline =
          e->cShape->circle.getOutlineColor();
      sf::Color outlineColor(currOutline.r, currOutline.g,
                             currOutline.b, opacity);
      e->cShape->circle.setOutlineColor(outlineColor);
    }
  }

  std::string scoreStr = "Score: " + std::to_string(m_score);
  if ( !m_font.loadFromFile("RedditMono-VariableFont_wght.ttf")){
    std::cout<<"couldn't load file\n";
    return;
  }
  m_text.setString(scoreStr);
  m_text.setFont(m_font);
  m_text.setCharacterSize(20);
  m_text.setFillColor(sf::Color::White);
  m_text.setPosition(m_window.getSize().x / 2 - m_text.getLocalBounds().width / 2, m_text.getLocalBounds().height);
  m_window.draw(m_text);
  m_window.display();
}
