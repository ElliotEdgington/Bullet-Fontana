#include "SFAsset.h"

int SFAsset::SFASSETID=0;

SFAsset::SFAsset(std::shared_ptr<SFWindow> window) : sf_window(window), alive(true){
  this->id   = ++SFASSETID;
}

SFAsset::SFAsset(const SFAsset& a) {
  sprite = a.sprite;
  sf_window = a.sf_window;
  bbox   = a.bbox;
}

SFAsset::~SFAsset() {
  bbox.reset();
  if(sprite) {
    SDL_DestroyTexture(sprite);
    sprite = nullptr;
  }
}

void SFAsset::LoadSprite(){
  if(!sprite) {
    cerr << "Could not load asset of type " << endl;
    throw SF_ERROR_LOAD_ASSET;
  }
  // Get texture width & height
  SDL_QueryTexture(sprite, NULL, NULL, &sprite_width, &sprite_height);
  // Initialise bounding box
  bbox = make_shared<SFBoundingBox>(SFBoundingBox(Vector2(0.0f, 0.0f), sprite_width, sprite_height));
}

/**
 * The logical coordinates in the game assume that the screen
 * is indexed from 0,0 in the bottom left corner.  The blittable
 * coordinates of the screen map 0,0 to the top left corner. We
 * need to convert between the two coordinate spaces.  We assume
 * that there is a 1-to-1 quantisation.
 */


void SFAsset::SetPosition(Point2 & point) {
  Vector2 v(point.getX(), point.getY());
  bbox->SetCentre(v);
}

Point2 SFAsset::GetPosition() {
  return Point2(bbox->centre->getX(), bbox->centre->getY());
}

SFAssetId SFAsset::GetId() {
  return id;
}

void SFAsset::OnRender() {
  // 1. Get the SDL_Rect from SFBoundingBox
  SDL_Rect rect;

  Vector2 gs = (*(bbox->centre) + (*(bbox->extent_x) * -1)) + (*(bbox->extent_y) * 1);
  Vector2 ss = GameSpaceToScreenSpace(sf_window->getRenderer(), gs);
  rect.x = ss.getX();
  rect.y = ss.getY();
  rect.w = bbox->extent_x->getX() * 2;
  rect.h = bbox->extent_y->getY() * 2;

  // 2. Blit the sprite onto the level
  SDL_RenderCopy(sf_window->getRenderer(), sprite, NULL, &rect);
}

// basic movement up down left right.
void SFAsset::GoWest() {
  Vector2 c = *(bbox->centre) + Vector2(-move_speed, 0.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

void SFAsset::GoEast() {
  Vector2 c = *(bbox->centre) + Vector2(move_speed, 0.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

void SFAsset::GoNorth() {
  Vector2 c = *(bbox->centre) + Vector2(0.0f, move_speed);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

void SFAsset::GoSouth(){
  Vector2 c = *(bbox->centre) + Vector2(0.0f, -move_speed);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

// vector2 param for more free movement
void SFAsset::GoDirection(Vector2 & v){
  Vector2 c = *(bbox->centre) + v*move_speed;
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

bool SFAsset::CollidesWith(shared_ptr<SFAsset> other) {
  return bbox->CollidesWith(other->bbox);
}

shared_ptr<SFBoundingBox> SFAsset::GetBoundingBox() {
  return bbox;
}

void SFAsset::SetNotAlive() {
  alive = false;
}

bool SFAsset::IsAlive() {
  return alive;
}

void SFAsset::HandleCollision() {
    health -= 1;
    if(health <= 0)SetNotAlive();
}
