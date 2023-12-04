#include "CoreMinimal.h"
#include "WorkerHubSwitchListener.generated.h"


UINTERFACE()
class UWorkerHubSwitchListener : public UInterface
{
	GENERATED_BODY()
};

class IWorkerHubSwitchListener
{
	GENERATED_BODY()
public:
	virtual void PressedWorkerHubSwitchButton(int32 WorkerHubNumber) = 0;

};