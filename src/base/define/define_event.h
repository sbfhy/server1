#pragma once

#define EVENT_CALLBACK(__selector__, __target__) std::bind(&__selector__, __target__, std::placeholders::_1, std::placeholders::_2)

