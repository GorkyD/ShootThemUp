#pragma once

#include "Animation/AnimSequenceBase.h"

class AnimUtils
{
  public:
	template <typename T> //
	static T* FindNotifyByClass(UAnimSequenceBase* Animation)
	{
		if (!Animation) return nullptr;

		const auto NotifyEvents = Animation->Notifies;
		for (auto notify : NotifyEvents)
			if (const auto AnimNotify = Cast<T>(notify.Notify)) return AnimNotify;

		return nullptr;
	};
};
