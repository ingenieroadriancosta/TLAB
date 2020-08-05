LRESULT TLAB_CLASS::WProcedure( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
    switch( message ){
        case WM_CREATE:
            CREATE( hwnd, message, wParam, lParam );
            break;
        case WM_COMMAND:
            CmdProcs( hwnd, message, wParam, lParam );
            break;
        case WM_CLOSE:
            return DefWindowProc( hwnd, message, wParam, lParam );
        case WM_DESTROY:
            WIN_ON = FALSE;
            PostQuitMessage( 0 );
            break;
        default:
            return DefWindowProc( hwnd, message, wParam, lParam );
            break;
    }
    return 0;
}
