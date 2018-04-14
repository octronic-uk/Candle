// This file is a part of "CoconutCNC" application.
// This file was originally ported from "ArcProperties.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "ArcProperties.h"

ArcProperties::ArcProperties()
    : isClockwise(false) ,
      radius(0),
      center(QVector3D())
{

}

ArcProperties::ArcProperties(const ArcProperties& other)
    : isClockwise(other.isClockwise),
      radius(other.radius),
      center(other.center)
{

}
