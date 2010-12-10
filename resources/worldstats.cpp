/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "asylum/resources/worldstats.h"

#include "asylum/resources/actor.h"
#include "asylum/resources/object.h"
#include "asylum/resources/script.h"

#include "asylum/asylum.h"

namespace Asylum {

WorldStats::WorldStats(AsylumEngine *engine) : _vm(engine) {
	// Init values
	size = 0;
	numEntries = 0;
	chapter = kChapterNone;
	xLeft = 0;
	yTop = 0;
	backgroundImage = kResourceNone;
	memset(&cursorResources, kResourceNone, sizeof(cursorResources));

	font1 = kResourceNone;
	font2 = kResourceNone;
	font3 = kResourceNone;
	currentPaletteId = kResourceNone;
	cellShadeMask1 = 0;
	cellShadeMask2 = 0;
	cellShadeMask3 = 0;
	unused = 0;
	smallCurUp = 0;
	smallCurDown = 0;
	encounterFrameBg = 0;
	width = 0;
	height = 0;
	motionStatus = 0;
	field_8C = 0;
	memset(&coordinates, 0, sizeof(coordinates));
	reverseStereo = 0;
	sceneRectIdx = 0;
	memset(&field_11D, 0, sizeof(field_11D));
	field_120 = 0;
	scriptIndex = 0;
	memset(&graphicResourceIds, kResourceNone, sizeof(graphicResourceIds));
	sceneTitleGraphicResourceId = kResourceNone;
	sceneTitlePaletteResourceId = kResourceNone;
	actorType = 0;
	memset(&soundResourceIds, kResourceNone, sizeof(soundResourceIds));

	numAmbientSounds = 0;
	musicStatus = 0;
	musicCurrentResourceIndex = 0;
	musicFlag = 0;
	musicResourceIndex = 0;
	musicStatusExt = 0;
	numScripts = 0;
	numPolygons = 0;
	memset(&cursorResourcesAlternate, kResourceNone, sizeof(cursorResourcesAlternate));

	nextPlayer = kActorMax;
	memset(&field_E8610, 0, sizeof(field_E8610));
	memset(&field_E8628, 0, sizeof(field_E8628));
	memset(&wheels, 0, sizeof(wheels));
	tickCount1 = 0;
	memset(&field_E8660, 0, sizeof(field_E8660));
}

WorldStats::~WorldStats() {
	CLEAR_ARRAY(Object, objects);
	CLEAR_ARRAY(Actor, actors);
	CLEAR_ARRAY(ActionArea, actions);

	// Zero-out passed pointers
	_vm = NULL;
}

// FIXME: load necessary World Stats content
void WorldStats::load(Common::SeekableReadStream *stream) {
	size       = stream->readSint32LE();
	numEntries = stream->readSint32LE();

	chapter = (ChapterIndex)stream->readSint32LE();
	xLeft      = stream->readSint32LE();
	yTop       = stream->readSint32LE();

	boundingRect.left   = (int16)(stream->readSint32LE() & 0xFFFF);
	boundingRect.top    = (int16)(stream->readSint32LE() & 0xFFFF);
	boundingRect.right  = (int16)(stream->readSint32LE() & 0xFFFF);
	boundingRect.bottom = (int16)(stream->readSint32LE() & 0xFFFF);

	// read common graphic resources
	backgroundImage     = (ResourceId)stream->readSint32LE();

	for (uint i = 0; i < ARRAYSIZE(cursorResources); i++)
		cursorResources[i] = (ResourceId)stream->readSint32LE();

	font1               = (ResourceId)stream->readSint32LE();
	font2               = (ResourceId)stream->readSint32LE();
	font3               = (ResourceId)stream->readSint32LE();
	currentPaletteId    = (ResourceId)stream->readSint32LE();
	cellShadeMask1      = stream->readSint32LE();
	cellShadeMask2      = stream->readSint32LE();
	cellShadeMask3      = stream->readSint32LE();
	unused              = stream->readSint32LE();
	smallCurUp          = stream->readSint32LE();
	smallCurDown        = stream->readSint32LE();
	encounterFrameBg    = stream->readSint32LE();

	width               = stream->readSint32LE();
	height              = stream->readSint32LE();
	motionStatus        = stream->readSint32LE();
	field_8C            = stream->readSint32LE();
	uint32 numActions   = stream->readUint32LE();
	uint32 numObjects   = stream->readUint32LE();

	for (int32 c = 0; c < 7; c++)
		coordinates[c] = stream->readSint32LE();

	uint32 numActors = stream->readUint32LE();

	reverseStereo = stream->readSint32LE();

	for (int32 r = 0; r < 6; r++) {
		sceneRects[r].left      = (int16)(stream->readSint32LE() & 0xFFFF);
		sceneRects[r].top       = (int16)(stream->readSint32LE() & 0xFFFF);
		sceneRects[r].right     = (int16)(stream->readSint32LE() & 0xFFFF);
		sceneRects[r].bottom    = (int16)(stream->readSint32LE() & 0xFFFF);
	}

	sceneRectIdx                = stream->readByte();
	field_11D[0]                = stream->readByte();
	field_11D[1]                = stream->readByte();
	field_11D[2]                = stream->readByte();
	field_120                   = stream->readSint32LE();
	scriptIndex                 = stream->readSint32LE();

	for (int32 gr = 0; gr < 100; gr++)
		graphicResourceIds[gr]  = (ResourceId)stream->readSint32LE();

	sceneTitleGraphicResourceId = (ResourceId)stream->readSint32LE();
	sceneTitlePaletteResourceId = (ResourceId)stream->readSint32LE();
	actorType                   = stream->readUint32LE();

	for (int32 s = 0; s < 50; s++)
		soundResourceIds[s] = (ResourceId)stream->readSint32LE();

	for (int32 s = 0; s < 15; s++) {
		ambientSounds[s].field_0  = stream->readSint32LE();
		ambientSounds[s].flags    = stream->readSint32LE();
		ambientSounds[s].resourceId    = (ResourceId)stream->readSint32LE();
		ambientSounds[s].delta  = stream->readSint32LE();
		ambientSounds[s].attenuation = stream->readSint32LE();
		ambientSounds[s].nextTick = stream->readSint32LE();

		for (int32 i = 0; i < 6; i++)
			ambientSounds[s].flagNum[i] = (GameFlag)stream->readSint32LE();

		ambientSounds[s].x = stream->readSint32LE();
		ambientSounds[s].y = stream->readSint32LE();
	}

	numAmbientSounds          = stream->readSint32LE();
	musicStatus               = stream->readSint32LE();
	musicCurrentResourceIndex = stream->readSint32LE();
	musicFlag                 = stream->readSint32LE();
	musicResourceIndex        = stream->readSint32LE();
	musicStatusExt            = stream->readSint32LE();

	//////////////////////////////////////////////////////////////////////////
	// Read Objects
	for (uint32 a = 0; a < numObjects; a++) {
		Object *object = new Object(_vm);
		object->load(stream);

		objects.push_back(object);
	}

	stream->seek((OBJECTS_MAX_COUNT - numObjects) * OBJECTS_SIZE, SEEK_CUR);

	//////////////////////////////////////////////////////////////////////////
	// Read Actors
	for (ActorIndex index = 0; index < (int)numActors; index++) {
		Actor *actor = new Actor(_vm, index);
		actor->load(stream);

		actors.push_back(actor);
	}

	stream->seek((ACTORS_MAX_COUNT - numActors) * ACTORS_SIZE, SEEK_CUR);

	//////////////////////////////////////////////////////////////////////////
	// Read actor data
	for (ActorIndex index = 0; index < (int)numActors; index++)
		actors[index]->loadData(stream);

	stream->seek((ACTORS_MAX_COUNT - numActors) * ACTORDATA_SIZE, SEEK_CUR);

	//////////////////////////////////////////////////////////////////////////
	// Read number of scripts and polygons
	numScripts = stream->readUint32LE();
	numPolygons    = stream->readUint32LE();

	// Load the alternate cursor resources
	for (uint32 i = 0; i < ARRAYSIZE(cursorResourcesAlternate); i++)
		cursorResourcesAlternate[i] = (ResourceId)stream->readSint32LE();

	//////////////////////////////////////////////////////////////////////////
	// Read actions
	for (uint32 a = 0; a < numActions; a++) {
		ActionArea *action = new ActionArea();
		action->load(stream);

		actions.push_back(action);
	}

	stream->seek((ACTIONS_MAX_COUNT - numActions) * ACTIONS_SIZE, SEEK_CUR);

	field_E848C = stream->readSint32LE();
	field_E8490 = stream->readSint32LE();
	field_E8494 = stream->readSint32LE();
	field_E8498 = stream->readSint32LE();
	field_E849C = stream->readSint32LE();

	for (int32 i = 0; i < ARRAYSIZE(tickValueArray); i++)
		tickValueArray[i] = stream->readSint32LE();

	field_E8518 = stream->readSint32LE();

	for (int32 i = 0; i < 30; i++)
		field_E851C[i] = stream->readSint32LE();

	for (int32 i = 0; i < 30; i++)
		field_E8594[i] = stream->readSint32LE();

	nextPlayer = (ActorIndex)stream->readSint32LE();

	for (int32 i = 0; i < 6; i++)
		field_E8610[i] = stream->readUint32LE();

	for (int32 i = 0; i < 6; i++)
		field_E8628[i] = stream->readUint32LE();

	for (int32 i = 0; i < 7; i++) {
		ObjectId id = (ObjectId)stream->readUint32LE();

		if (id == 0)
			wheels[i] = NULL;
		else
			wheels[i] = getObjectById(id);
	}

	tickCount1 = stream->readSint32LE();

	for (int32 i = 0; i < 6; i++)
		field_E8660[i] = stream->readUint32LE();
}

//////////////////////////////////////////////////////////////////////////
// Helper methods
//////////////////////////////////////////////////////////////////////////
int32 WorldStats::getActionAreaIndexById(int32 id) {
	for (uint32 i = 0; i < actions.size(); i++) {
		if (actions[i]->id == id)
			return i;
	}

	return -1;
}

int32 WorldStats::getRandomActionAreaIndexById(int32 id) {
	int count = 0;
	int32 indexes[5];
	memset(&indexes, 0, sizeof(indexes));

	for (uint32 i = 0; i < actions.size(); i++) {
		if (actions[i]->id == id && count < 5) {
			indexes[count] = i;
			++count;
		}
	}

	if (!count)
		return -1;

	return indexes[_vm->getRandom(count)];
}

ActionArea* WorldStats::getActionAreaById(int32 id) {
	int index = getActionAreaIndexById(id);
	if (index == -1)
		error("[WorldStats::getActionAreaById] action id is invalid");

	return actions[index];
}

Object* WorldStats::getObjectById(ObjectId id) {
	for (uint32 i = 0; i < objects.size(); i++)
		if (objects[i]->getId() == id)
			return objects[i];

	return NULL;
}

void WorldStats::setWheelObjects() {
	wheels[0] = getObjectById(kObjectWheel1);
	wheels[1] = getObjectById(kObjectWheel2);
	wheels[2] = getObjectById(kObjectWheel3);
	wheels[3] = getObjectById(kObjectWheel4);
	wheels[4] = getObjectById(kObjectWheel5);
	wheels[5] = getObjectById(kObjectWheel6);
	wheels[6] = getObjectById(kObjectWheel7);
}

Common::String WorldStats::toString() {
	Common::String output;

	output += Common::String::format("xLeft:          %d\n", xLeft);
	output += Common::String::format("yTop:           %d\n", yTop);
	output += Common::String::format("boundingRect:   top[%d] left[%d] right[%d] bottom[%d]: \n", boundingRect.top, boundingRect.left, boundingRect.right, boundingRect.bottom);
	output += Common::String::format("width:          %d\n", width);
	output += Common::String::format("height:         %d\n", height);
	output += Common::String::format("motionStatus:   %d\n", motionStatus);
	output += "coordinates:    ";
	for (int i = 0; i < 7; i++)
		output += Common::String::format("%d[%d] ", i, coordinates[i]);
	output += "\n";
	output += Common::String::format("sceneRectIndex: %d\n", sceneRectIdx);
	output += Common::String::format("sceneRects:     0: top[%d] left[%d] right[%d] bottom[%d]\n", sceneRects[0].top, sceneRects[0].left, sceneRects[0].right, sceneRects[0].bottom);
	for (int i = 1; i < 6; i++)
		output += Common::String::format("                %d: top[%d] left[%d] right[%d] bottom[%d]\n", i, sceneRects[i].top, sceneRects[i].left, sceneRects[i].right, sceneRects[i].bottom);
	output += Common::String::format("scriptInex:     %d\n", scriptIndex);
	output += Common::String::format("actorType:      %d\n", actorType);
	output += Common::String::format("musicStatus:    %d\n", musicStatus);

	return output;
}

void WorldStats::saveLoadWithSerializer(Common::Serializer &s) {
	error("[WorldStats::saveLoadWithSerializer] Not implemented!");
}

} // end of namespace Asylum
