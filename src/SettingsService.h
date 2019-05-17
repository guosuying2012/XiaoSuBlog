#ifndef SETTING_SERVICE_H
#define SETTING_SERVICE_H

#include "BaseService.h"

class SettingsService : public BaseService
{
public:
    SettingsService(cppcms::service& srv);
    ~SettingsService();

private:
    virtual void index() override;
    void navigationBar();
    void sliderImages();
    void websiteOptions();
};

#endif  //SettingService.h