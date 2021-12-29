#pragma once

#include <memory>

class User;
typedef std::shared_ptr<User> UserPtr;
typedef std::weak_ptr<User> UserWPtr;
