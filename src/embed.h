#ifndef EMBED_TEMPLATES_H
#define EMBED_TEMPLATES_H

#include <dpp/dpp.h>

class EmbedTemplates {
public:
    
    static dpp::embed createBasicEmbed(
        const std::string& title, 
        const std::string& description,
        const std::string& url = "https://dpp.dev/",
        const std::string& thumbnail = "https://dpp.dev/DPP-Logo.png",
        const std::string& footer_text = "Some footer text here"
    );
};

#endif // EMBED_TEMPLATES_H