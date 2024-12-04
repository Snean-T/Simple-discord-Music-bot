#include <iostream>
#include <dpp/dpp.h>
#include <mpg123.h>
#include "Bot_Token.h"
#include "embed.h"
#include <unordered_map>






namespace fs = std::filesystem;

// Helper function to list .mp3 files in a directory
std::vector<std::string> list_mp3_files(const std::string& dir_path) {
    std::vector<std::string> files;
    if (fs::exists(dir_path) && fs::is_directory(dir_path)) {
        for (const auto& entry : fs::directory_iterator(dir_path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".mp3") {
                files.push_back(entry.path().filename().string());
            }
        }
    }
    return files;
}

  









 
int main() {
    std::unordered_map<std::string, std::string> file_path_map;
    
    int err = 0;
    mpg123_init();
    mpg123_handle *mh = mpg123_new(NULL, &err);
    

    //cache the message id send for further use.//
    dpp::snowflake sent_message_id;

    

    
    /*Cluster is DPP's main thread*/
    dpp::cluster bot(BOT_TOKEN,dpp::i_default_intents);

    /*on_log which create a log file*/
    bot.on_log(dpp::utility::cout_logger());
    
    
    bot.on_slashcommand([&bot](const dpp::slashcommand_t &event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
        else if (event.command.get_command_name() == "embed") {
            dpp::embed embed = EmbedTemplates::createBasicEmbed(
            "Some name", 
            "Some description here"
            );

            dpp::message msg(event.command.channel_id, embed);
            event.reply(msg);
        };
    });

    bot.on_slashcommand([&bot,&file_path_map,&sent_message_id,&mh](const dpp::slashcommand_t &event) {
        /* Check which command they ran */
        if (event.command.get_command_name() == "join") {
            /* Get the guild */
            dpp::guild* g = dpp::find_guild(event.command.guild_id);
 
            /* Attempt to connect to a voice channel, returns false if we fail to connect. */
            if (!g->connect_member_voice(event.command.get_issuing_user().id)) {
                event.reply("You don't seem to be in a voice channel!");
                return;
            }
            
            /* Tell the user we joined their channel. */
            event.reply("Joined your channel!");
        }else if(event.command.get_command_name() == "mp3") {
            /* Get the voice channel the bot is in, in this current guild. */
            dpp::voiceconn* v = event.from->get_voice(event.command.guild_id);
            
            //get the parameter of the mp3 file path//
            std::string folder_path = std::get<std::string>(event.get_parameter("folder"));
            std::vector<std::string> files = list_mp3_files(folder_path);
 
            /* If the voice channel was invalid, or there is an issue with it, then tell the user. */
            if (!v || !v->voiceclient || !v->voiceclient->is_ready()) {
                event.reply("There was an issue with getting the voice channel. Make sure I'm in a voice channel!");
                return;
            }

            

            if (files.empty()) {
                event.reply("No .mp3 files found in the specified directory or the directory is invalid.");
                return;
            }

            // Create a dropdown menu for .mp3 file selection
            dpp::component file_dropdown;
            file_dropdown.set_type(dpp::cot_selectmenu)
                         .set_id("mp3_file_select")
                         .set_placeholder("Select an .mp3 file");

            // Add files as options to the dropdown menu
            for (size_t i = 0; i < files.size(); ++i) {
                std::string key = "file_" + std::to_string(i); // Generate a unique key
                file_path_map[key] = folder_path + "/" + files[i]; // Map key to full path
                
                file_dropdown.add_select_option(
                    dpp::select_option(files[i], key) // Use the short key as value
                );
            }

            // Send the dropdown menu as a response
            dpp::message msg("Select an .mp3 file from the directory:");
            msg.add_component(dpp::component().add_component(file_dropdown));
            event.reply(msg);
            
            bot.message_create(msg, [&sent_message_id](const dpp::confirmation_callback_t& callback) {
            if (callback.is_error()) {
                return; // return error if the callback has error
            }
            sent_message_id = std::get<dpp::message>(callback.value).id;
            });
            
            
 
            
        }
        if (event.command.get_command_name() == "pause_or_resume"){
            dpp::voiceconn* v = event.from->get_voice(event.command.guild_id);
            if (v->voiceclient->is_paused()) {
                v->voiceclient->pause_audio(false);
                event.reply("the audio is play");
            } else{
                v->voiceclient->pause_audio(true);
                event.reply("the audio is pause");
            }
        }
        
        if (event.command.get_command_name() == "leave") {
            
            dpp::voiceconn* v = event.from->get_voice(event.command.guild_id);
            
            if ((v -> voiceclient)) 
            {
                event.from->disconnect_voice(event.command.guild_id);
                event.reply("left the channel.");
            } else {
            event.reply("You are not in a voice channel.");
        };

        
        
        

    

    };
        
    });
    
    bot.on_select_click([&bot,&file_path_map,sent_message_id,&mh](const dpp::select_click_t& event) {
        if (event.custom_id == "mp3_file_select") {
            std::string selected_key = event.values[0]; // Get the short key
            std::string selected_file_path = file_path_map[selected_key]; // Retrieve full path

            char* selected_path = selected_file_path.data();
           

            event.reply("You selected: `" + selected_file_path + "`" + 
                        "\n Played the file.");

            dpp::voiceconn* v = event.from->get_voice(event.command.guild_id);
            std::vector<uint8_t> pcmdata;
            std::vector<unsigned char> prebuffer;
            
            unsigned char* buffer;
            size_t buffer_size, done;
            int channels, encoding;
            long rate;
 
            /* Note it is important to force the frequency to 48000 for Discord compatibility */
            
            mpg123_param(mh, MPG123_FORCE_RATE, 48000, 48000.0);
 
            /* Decode entire file into a vector. You could do this on the fly, but if you do that
            * you may get timing issues if your CPU is busy at the time and you are streaming to
            * a lot of channels/guilds.
            */
            buffer_size = mpg123_outblock(mh);
            buffer = new unsigned char[buffer_size];
            //the prebuffer initiallization
            prebuffer.reserve(buffer_size);

            // save the total byte that already decoded
            unsigned int totalBytes = 0;
 
            
           
            mpg123_open(mh, selected_path);
          
            
            mpg123_getformat(mh, &rate, &channels, &encoding);
           
         
            while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK ) {
                if (prebuffer.size() + done > buffer_size) {
                    size_t overflow = (prebuffer.size() + done) - buffer_size;
                    prebuffer.erase(prebuffer.begin(), prebuffer.begin() + overflow);
                }

                // put the data that decode to prebuffer
                prebuffer.insert(prebuffer.end(), buffer, buffer + done);

                if (!prebuffer.empty())
                {
                // put the prebuffer data to pcmdata 
                pcmdata.insert(pcmdata.end(),prebuffer.begin(),prebuffer.end());
                
                totalBytes += done; // until the totalBytes done
                }
            
            }
        
        delete[] buffer; //release the memory
        prebuffer.clear();
        totalBytes = 0;
        mpg123_close(mh);
        mh = nullptr;
                
            

        /* Stream the already decoded MP3 file. This passes the PCM data to the library to be encoded to OPUS */
        v->voiceclient->send_audio_raw((uint16_t*)pcmdata.data(), pcmdata.size());
            
            
    }});
 
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            /*!Remeber to check the command name you registered before, otherwise it will occur command error*/
            dpp::slashcommand Pingcommand ("ping", "Ping Pong!", bot.me.id);
            dpp::slashcommand embedcommand ("embed", "Send a test embed!", bot.me.id);
            dpp::slashcommand joincommand ("join", "join the sound channel", bot.me.id);
            dpp::slashcommand leavecommand ("leave", "leave the sound channel", bot.me.id);
            dpp::slashcommand mp3command("mp3", "play the mp3 file", bot.me.id);
                mp3command.add_option(dpp::command_option(dpp::co_string, "folder", "The directory path to browse", true));
            dpp::slashcommand PauseOrPlaycommand("pause_or_resume", "pause the audio", bot.me.id);
            
            /*use bulk_command to create multi command otherwise it will occur error*/
            bot.global_bulk_command_create({ Pingcommand , embedcommand ,
             joincommand , leavecommand, mp3command, PauseOrPlaycommand,});
        }
    });
 
    bot.start(dpp::st_wait);

    mpg123_exit();

    return 0;
}