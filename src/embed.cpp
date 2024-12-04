#include "embed.h"
#include <ctime>

dpp::embed EmbedTemplates::createBasicEmbed(
    const std::string& title, 
    const std::string& description,
    const std::string& url,
    const std::string& thumbnail,
    const std::string& footer_text
) {
    return dpp::embed()
        .set_color(dpp::colors::sti_blue)
        .set_title(title)
        .set_url(url)
        .set_author("Some name", url, thumbnail)
        .set_description(description)
        .set_thumbnail(thumbnail)
        .add_field("Regular field title", "Some value here")
        .add_field("Inline field title", "Some value here", true)
        .add_field("Inline field title", "Some value here", true)
        .set_image(thumbnail)
        .set_footer(dpp::embed_footer().set_text(footer_text).set_icon(thumbnail))
        .set_timestamp(time(0));
}