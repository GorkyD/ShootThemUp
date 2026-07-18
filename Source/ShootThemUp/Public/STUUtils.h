#pragma once

class FStuUtils
{
  public:
	template <typename T> static T* GetSTUPlayerComponent(AActor* Player)
	{
		if (!Player) return nullptr;

		const auto Component = Player->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};