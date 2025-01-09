// Copyright by MykeUhu


#include "UI/Portal/SignIn/SignInOverlay.h"

#include "UI/Portal/PortalManager.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	check(PortalManagerClass);
	
	PortalManager = NewObject<UPortalManager>(PortalManagerClass);
}
