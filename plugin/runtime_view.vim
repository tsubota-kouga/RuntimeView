
scriptencoding utf-8
if exists('g:loaded_runtimeview')
    finish
endif
let g:loaded_runtimeview = 1
if !exists('g:runtime_view#css_name')
    let g:runtime_view#css_name = 'default'
endif
setlocal runtimepath+=.

let s:save_cpo = &cpo
set cpo&vim

command! -nargs=0 RuntimeView call runtime_view#runtimeview_start()

command! -nargs=0 LoadView call runtime_view#loadview_start()

let &cpo = s:save_cpo
unlet s:save_cpo

