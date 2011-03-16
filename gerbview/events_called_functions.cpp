/**********************************************************/
/* events_called_functions.cpp: Gerbview events functions */
/**********************************************************/

#include "fctsys.h"
#include "class_drawpanel.h"
#include "confirm.h"
#include "common.h"
#include "gestfich.h"
#include "appl_wxstruct.h"

#include "gerbview.h"
#include "kicad_device_context.h"
#include "gerbview_id.h"
#include "class_GERBER.h"
#include "dialog_helpers.h"
#include "class_DCodeSelectionbox.h"

// Event table:

BEGIN_EVENT_TABLE( GERBVIEW_FRAME, PCB_BASE_FRAME )
EVT_CLOSE( GERBVIEW_FRAME::OnCloseWindow )
EVT_SIZE( GERBVIEW_FRAME::OnSize )

EVT_TOOL( wxID_FILE, GERBVIEW_FRAME::Files_io )
EVT_TOOL( ID_INC_LAYER_AND_APPEND_FILE, GERBVIEW_FRAME::Files_io )
EVT_TOOL( ID_GERBVIEW_LOAD_DRILL_FILE, GERBVIEW_FRAME::Files_io )
EVT_TOOL( ID_GERBVIEW_LOAD_DCODE_FILE, GERBVIEW_FRAME::Files_io )
EVT_TOOL( ID_NEW_BOARD, GERBVIEW_FRAME::Files_io )

// Menu Files:
EVT_MENU( wxID_FILE, GERBVIEW_FRAME::Files_io )
EVT_MENU( ID_MENU_INC_LAYER_AND_APPEND_FILE, GERBVIEW_FRAME::Files_io )
EVT_MENU( ID_NEW_BOARD, GERBVIEW_FRAME::Files_io )
EVT_MENU( ID_GEN_PLOT, GERBVIEW_FRAME::ToPlotter )
EVT_MENU( ID_GERBVIEW_EXPORT_TO_PCBNEW, GERBVIEW_FRAME::ExportDataInPcbnewFormat )

EVT_MENU_RANGE( wxID_FILE1, wxID_FILE9, GERBVIEW_FRAME::OnFileHistory )

EVT_MENU( ID_EXIT, GERBVIEW_FRAME::Process_Special_Functions )

// menu Preferences
EVT_MENU_RANGE( ID_PREFERENCES_HOTKEY_START, ID_PREFERENCES_HOTKEY_END,
                GERBVIEW_FRAME::Process_Config )

EVT_MENU( ID_MENU_GERBVIEW_SHOW_HIDE_LAYERS_MANAGER_DIALOG,
          GERBVIEW_FRAME::OnSelectOptionToolbar )
EVT_MENU( ID_GERBVIEW_OPTIONS_SETUP, GERBVIEW_FRAME::InstallGerberOptionsDialog )

EVT_MENU_RANGE( ID_LANGUAGE_CHOICE, ID_LANGUAGE_CHOICE_END, EDA_DRAW_FRAME::SetLanguage )

// menu Postprocess
EVT_MENU( ID_GERBVIEW_SHOW_LIST_DCODES, GERBVIEW_FRAME::Process_Special_Functions )
EVT_MENU( ID_GERBVIEW_SHOW_SOURCE, GERBVIEW_FRAME::OnShowGerberSourceFile )
EVT_MENU( ID_MENU_GERBVIEW_SELECT_PREFERED_EDITOR,
          EDA_BASE_FRAME::OnSelectPreferredEditor )

// menu Miscellaneous
EVT_MENU( ID_GERBVIEW_GLOBAL_DELETE, GERBVIEW_FRAME::Process_Special_Functions )

// Menu Help
EVT_MENU( ID_GENERAL_HELP, EDA_DRAW_FRAME::GetKicadHelp )
EVT_MENU( ID_KICAD_ABOUT, EDA_DRAW_FRAME::GetKicadAbout )

EVT_TOOL( wxID_CUT, GERBVIEW_FRAME::Process_Special_Functions )
EVT_TOOL( wxID_COPY, GERBVIEW_FRAME::Process_Special_Functions )
EVT_TOOL( wxID_PASTE, GERBVIEW_FRAME::Process_Special_Functions )
EVT_TOOL( wxID_UNDO, GERBVIEW_FRAME::Process_Special_Functions )
EVT_TOOL( wxID_PRINT, GERBVIEW_FRAME::ToPrinter )
EVT_TOOL( ID_FIND_ITEMS, GERBVIEW_FRAME::Process_Special_Functions )
EVT_KICAD_CHOICEBOX( ID_TOOLBARH_GERBVIEW_SELECT_ACTIVE_LAYER,
                     GERBVIEW_FRAME::OnSelectActiveLayer )

EVT_SELECT_DCODE( ID_TOOLBARH_GERBER_SELECT_ACTIVE_DCODE, GERBVIEW_FRAME::OnSelectActiveDCode )

// Vertical toolbar:
EVT_TOOL( ID_NO_TOOL_SELECTED, GERBVIEW_FRAME::Process_Special_Functions )

EVT_MENU_RANGE( ID_POPUP_GENERAL_START_RANGE, ID_POPUP_GENERAL_END_RANGE,
                GERBVIEW_FRAME::Process_Special_Functions )

// Option toolbar
EVT_TOOL( ID_TB_OPTIONS_SHOW_POLYGONS_SKETCH,
          GERBVIEW_FRAME::OnSelectOptionToolbar )
EVT_TOOL( ID_TB_OPTIONS_SHOW_FLASHED_ITEMS_SKETCH, GERBVIEW_FRAME::OnSelectOptionToolbar )
EVT_TOOL( ID_TB_OPTIONS_SHOW_LINES_SKETCH, GERBVIEW_FRAME::OnSelectOptionToolbar )
EVT_TOOL( ID_TB_OPTIONS_SHOW_LAYERS_MANAGER_VERTICAL_TOOLBAR,
          GERBVIEW_FRAME::OnSelectOptionToolbar )
EVT_TOOL( ID_TB_OPTIONS_SHOW_DCODES, GERBVIEW_FRAME::OnSelectOptionToolbar )
EVT_TOOL_RANGE( ID_TB_OPTIONS_SHOW_GBR_MODE_0, ID_TB_OPTIONS_SHOW_GBR_MODE_2,
                GERBVIEW_FRAME::OnSelectDisplayMode )

EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_FLASHED_ITEMS_SKETCH,
               GERBVIEW_FRAME::OnUpdateFlashedItemsDrawMode )
EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_LINES_SKETCH, GERBVIEW_FRAME::OnUpdateLinesDrawMode )
EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_POLYGONS_SKETCH,
               GERBVIEW_FRAME::OnUpdatePolygonsDrawMode )
EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_DCODES, GERBVIEW_FRAME::OnUpdateShowDCodes )
EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_LAYERS_MANAGER_VERTICAL_TOOLBAR,
               GERBVIEW_FRAME::OnUpdateShowLayerManager )

EVT_UPDATE_UI( ID_TOOLBARH_GERBER_SELECT_ACTIVE_DCODE, GERBVIEW_FRAME::OnUpdateSelectDCode )
EVT_UPDATE_UI( ID_TOOLBARH_GERBVIEW_SELECT_ACTIVE_LAYER, GERBVIEW_FRAME::OnUpdateLayerSelectBox )
EVT_UPDATE_UI_RANGE( ID_TB_OPTIONS_SHOW_GBR_MODE_0, ID_TB_OPTIONS_SHOW_GBR_MODE_2,
                     GERBVIEW_FRAME::OnUpdateDrawMode )

END_EVENT_TABLE()

/* Handles the selection of tools, menu, and popup menu commands.
 */
void GERBVIEW_FRAME::Process_Special_Functions( wxCommandEvent& event )
{
    int           id = event.GetId();

    switch( id )
    {
    case wxID_CUT:
    case wxID_COPY:
    case ID_POPUP_DELETE_BLOCK:
    case ID_POPUP_PLACE_BLOCK:
    case ID_POPUP_ZOOM_BLOCK:
        break;

    case ID_POPUP_CANCEL_CURRENT_COMMAND:
        DrawPanel->EndMouseCapture();

        if( GetScreen()->m_BlockLocate.m_Command != BLOCK_IDLE )
        {
            /* Should not be executed, except bug */
            GetScreen()->m_BlockLocate.m_Command = BLOCK_IDLE;
            GetScreen()->m_BlockLocate.m_State   = STATE_NO_BLOCK;
            GetScreen()->m_BlockLocate.ClearItemsList();
        }

        if( GetToolId() == ID_NO_TOOL_SELECTED )
            SetToolID( ID_NO_TOOL_SELECTED, DrawPanel->GetDefaultCursor(), wxEmptyString );
        else
            DrawPanel->SetCursor( DrawPanel->GetCurrentCursor() );
        break;

    default:
        DrawPanel->EndMouseCapture();
        break;
    }

    INSTALL_UNBUFFERED_DC( dc, DrawPanel );

    switch( id )
    {
    case ID_EXIT:
        Close( true );
        break;

    case ID_GERBVIEW_GLOBAL_DELETE:
        Erase_Current_Layer( true );
        ClearMsgPanel();
        break;

    case ID_NO_TOOL_SELECTED:
        SetToolID( ID_NO_TOOL_SELECTED, DrawPanel->GetDefaultCursor(), wxEmptyString );
        break;

    case ID_POPUP_CLOSE_CURRENT_TOOL:
        SetToolID( ID_NO_TOOL_SELECTED, DrawPanel->GetDefaultCursor(), wxEmptyString );
        break;

    case ID_POPUP_CANCEL_CURRENT_COMMAND:
        break;

    case ID_GERBVIEW_SHOW_LIST_DCODES:
        Liste_D_Codes();
        break;

    case ID_POPUP_PLACE_BLOCK:
        GetScreen()->m_BlockLocate.m_Command = BLOCK_MOVE;
        DrawPanel->m_AutoPAN_Request = FALSE;
        HandleBlockPlace( &dc );
        break;

    case ID_POPUP_ZOOM_BLOCK:
        GetScreen()->m_BlockLocate.m_Command = BLOCK_ZOOM;
        GetScreen()->m_BlockLocate.SetMessageBlock( this );
        HandleBlockEnd( &dc );
        break;

    case ID_POPUP_DELETE_BLOCK:
        GetScreen()->m_BlockLocate.m_Command = BLOCK_DELETE;
        GetScreen()->m_BlockLocate.SetMessageBlock( this );
        HandleBlockEnd( &dc );
        break;

    default:
        wxFAIL_MSG( wxT( "GERBVIEW_FRAME::Process_Special_Functions error" ) );
        break;
    }
}


/* Selects the active DCode for the current active layer.
 *  Items using this DCode are hightlighted
 */
void GERBVIEW_FRAME::OnSelectActiveDCode( wxCommandEvent& event )
{
    GERBER_IMAGE* gerber_image = g_GERBER_List[getActiveLayer()];
    if( gerber_image )
    {
        int tool = m_DCodeSelector->GetSelectedDCodeId();
        if( tool != gerber_image->m_Selected_Tool )
        {
            gerber_image->m_Selected_Tool = tool;
            DrawPanel->Refresh();
        }
    }
}

/* Selects the active layer:
 *  - if a file is loaded, it is loaded in this layer
 *  _ this layer is displayed on top of other layers
 */
void GERBVIEW_FRAME::OnSelectActiveLayer( wxCommandEvent& event )
{
    int layer = getActiveLayer();

    setActiveLayer( event.GetSelection() );
    if( layer != getActiveLayer() )
        DrawPanel->Refresh();
}


/* Call preferred editor to show (and edit) the gerber source file
 * loaded in the active layer
 */
void GERBVIEW_FRAME::OnShowGerberSourceFile( wxCommandEvent& event )
{
    int           layer = getActiveLayer();
    GERBER_IMAGE* gerber_layer = g_GERBER_List[layer];

    if( gerber_layer )
    {
        wxString editorname = wxGetApp().GetEditorName();
        if( !editorname.IsEmpty() )
        {
            wxFileName fn( gerber_layer->m_FileName );
            ExecuteFile( this, editorname, QuoteFullPath( fn ) );
        }
        else
        {
            wxMessageBox( _( "No editor defined. Please select one" ) );
        }
    }
}


/* Function OnSelectDisplayMode: called to select display mode
 * (fast display, or exact mode with stacked images or with transparency
 */
void GERBVIEW_FRAME::OnSelectDisplayMode( wxCommandEvent& event )
{
    int oldMode = GetDisplayMode();

    switch( event.GetId() )
    {
    case ID_TB_OPTIONS_SHOW_GBR_MODE_0:
        SetDisplayMode( 0 );
        break;

    case ID_TB_OPTIONS_SHOW_GBR_MODE_1:
        SetDisplayMode( 1 );
        break;

    case ID_TB_OPTIONS_SHOW_GBR_MODE_2:
        SetDisplayMode( 2 );
        break;
    }

    if( GetDisplayMode() != oldMode )
        DrawPanel->Refresh();
}


/**
 * Function SetLanguage
 * called on a language menu selection
 * Update Layer manager title and tabs texts
 */
void GERBVIEW_FRAME::SetLanguage( wxCommandEvent& event )
{
    EDA_DRAW_FRAME::SetLanguage( event );
    m_LayersManager->SetLayersManagerTabsText();
    wxAuiPaneInfo& pane_info = m_auimgr.GetPane( m_LayersManager );
    pane_info.Caption( _( "Visibles" ) );
    m_auimgr.Update();

    ReFillLayerWidget();
}