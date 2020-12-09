
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Input_box   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Input_box:  public Abstract_window
{
public:
    struct Settings {
        Point start = {200, 200};
        Point size = {500, 150};
        Color fill_color = WHITE;
        Color line_color = GREEN;
        int thickness = 20;


        Point letters_box_size = {300, 50};
        int letter_box_thickness = 3;
    };

private:
    Settings settings;

    std::string& str_answer;
    Text answer;
    Text question;
    Text ok;

    int shown_letters = 10;

    struct Stop_input {
        void operator () () {
            add_event (new Program_close ());
        }
    };

    Button<Stop_input> ok_button;

    Input_box (const std::string& question, std::string& answer, Settings settings);

    void run ();
public:
    static std::string make_input (const std::string& question);
    static std::string make_input (Input_box::Settings settings, const std::string& question);

    virtual void render ();
    virtual bool process_event (Event* event);
    virtual ~Input_box () = default;
};
