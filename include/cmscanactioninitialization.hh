#ifndef cmscanactioninitialization_h
#define cmscanactioninitialization_h


#include <string>
#include "G4VUserActionInitialization.hh"


class CMScanActionInitialization : public G4VUserActionInitialization {

public:
    explicit CMScanActionInitialization(std::string output_name);

    ~CMScanActionInitialization() override;

    void BuildForMaster() const override;

    void Build() const override;

private:
    std::string output_name_;
};

#endif