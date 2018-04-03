/*
 * GrblResponse.cpp
 *
 * Created: 03 2018 by Ashley
 *
 * Copyright 2018 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#include "GrblResponse.h"

GrblResponse::GrblResponse()
{

}

GrblResponse::GrblResponse(QString response)
    : mData(response)
{
    identifyType();
}

GrblResponse::~GrblResponse()
{

}

GrblResponseType GrblResponse::getType()
{
   return mType;
}

void GrblResponse::identifyType()
{
    if (mData.startsWith("Grbl"))
    {
        mType = GrblResponseType::Startup;
        return;
    }
    if (mData.startsWith("<") && mData.endsWith(">"))
    {
        mType = GrblResponseType::Status;
        return;
    }
    if (mData.contains("ok"))
    {
        mType = GrblResponseType::Ok;
        return;
    }
    if (mData.contains("error"))
    {
        mType = GrblResponseType::Error;
    }
    if (mData.contains("ALARM"))
    {
        mType = GrblResponseType::Alarm;
        return;
    }
    if (mData.contains("to unlock"))
    {
        mType = GrblResponseType::Locked;
        return;
    }
    if (mData.contains("HLP"))
    {
        mType = GrblResponseType::Help;
        return;
    }
    if (mData.contains("Unlocked"))
    {
        mType = GrblResponseType::Unlocked;
        return;
    }
    if (mData.contains("[Enabled]"))
    {
        mType = GrblResponseType::Enabled;
        return;
    }
    if (mData.contains("[Disabled]"))
    {
        mType = GrblResponseType::Disabled;
        return;
    }
    if (mData.contains("PRB"))
    {
        mType = GrblResponseType::Probe;
        return;
    }
    mType = GrblResponseType::ParserState;
}

QString GrblResponse::getData()
{
    return mData;
}
