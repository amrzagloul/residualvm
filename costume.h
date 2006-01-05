// Residual - Virtual machine to run LucasArts' 3D adventure games
// Copyright (C) 2003-2005 The ScummVM-Residual Team (www.scummvm.org)
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

#ifndef COSTUME_H
#define COSTUME_H

#include "matrix4.h"
#include "model.h"

#include <string>

#define DEFAULT_COLORMAP "item.cmp"

typedef uint32 tag32;

class TextSplitter;
class Actor;

class Costume {
public:
	Costume(const char *filename, const char *data, int len, Costume *prevCost);

	~Costume();

	const char *filename() const { return _fname.c_str(); }
	void playChore(int num);
	void playChoreLooping(int num);
	void setChoreLastFrame(int num) { _chores[num].setLastFrame(); }
	void setChoreLooping(int num, bool val) { _chores[num].setLooping(val); }
	void stopChore(int num) { _chores[num].stop(); }
	Model::HierNode *getModelNodes();
	void setColormap(char *map);
	void stopChores();
	int isChoring(char *name, bool excludeLooping);
	int isChoring(int num, bool excludeLooping);
	int isChoring(bool excludeLooping);

	void setHead(int joint1, int joint2, int joint3, float maxRoll, float maxPitch, float maxYaw);

	void update();
	void setupTextures();
	void draw();
	void setPosRotate(Vector3d pos, float pitch, float yaw, float roll);

	class Component {
	public:
		Component(Component *parent, int parentID, tag32 tag);

		tag32 tag() { return _tag; }
		CMap *cmap();
		void setColormap(CMap *c);
		bool visible();
		Component *parent() { return _parent; }
		virtual void setMatrix(Matrix4) { };
		virtual void init() { }
		virtual void setKey(int) { }
		virtual void setMapName(char *) { }
		virtual void update() { }
		virtual void setupTexture() { }
		virtual void draw() { }
		virtual void reset() { }
		virtual ~Component() { }

	protected:
		ResPtr<CMap> _cmap, _previousCmap;
		tag32 _tag;
		int _parentID, _visible;
		Component *_parent, *_child, *_sibling;
		Matrix4 _matrix;
		Costume *_cost;
		void setCostume(Costume *cost) { _cost = cost; }
		void setParent(Component *newParent);

		friend class Costume;
	};

private:
	Component *loadComponent(tag32 tag, Component *parent, int parentID, const char *name, Component *prevComponent);
	std::string _fname;
	
	int _numComponents;
	Component **_components;

	struct TrackKey {
		int time, value;
	};

	struct ChoreTrack {
		int compID;
		int numKeys;
		TrackKey *keys;
	};

	struct Head {
		int joint1;
		int joint2;
		int joint3;
		float maxRoll;
		float maxPitch;
		float maxYaw;
	} _head;

	class Chore {
	public:
		Chore();
		void load(Costume *owner, TextSplitter &ts);
		void play();
		void playLooping();
		void setLooping(bool val) { _looping = val; }
		void stop();
		void update();
		void setLastFrame();

	private:
		Costume *_owner;

		int _length;
		int _numTracks;
		ChoreTrack *_tracks;
		char _name[32];

		bool _hasPlayed, _playing, _looping;
		int _currTime;

		void setKeys(int startTime, int stopTime);

		friend class Costume;
	};

	ResPtr<CMap> _cmap;
	int _numChores;
	Chore *_chores;
	Matrix4 _matrix;
};

#endif
