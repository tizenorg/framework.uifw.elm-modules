#include <Elementary.h>
#include <tts.h>

/* to enable this module
export ELM_MODULES="access_output_tts>access/api"
export ELM_ACCESS_MODE=1
 */

static tts_h tts;

void _tts_state_changed_cb(tts_h tts, tts_state_e previous, tts_state_e current, void* data)
{
	//TODO
}

EAPI int
elm_modapi_init(void *m )
{
   int ret = 0;
   if (tts)
     {
        free(tts);
        tts = NULL;
     }

   ret = tts_create(&tts);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to get handle : result(%d)", ret);
        return ret;
     }

   ret = tts_set_state_changed_cb(tts, _tts_state_changed_cb, NULL);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to set callback : result(%d)", ret);
        return ret;
     }

   ret = tts_prepare(tts);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to prepare handle : result(%d)", ret);
        return ret;
     }

   return 1;
}

EAPI int
elm_modapi_shutdown(void *m )
{
   int ret = 0;
   if (tts)
     {
        ret = tts_unprepare(tts);
        if (TTS_ERROR_NONE != ret)
          {
             fprintf(stderr, "Fail to unprepare handle : result(%d)", ret);
             return ret;
          }

        ret = tts_destroy(tts);
        if (TTS_ERROR_NONE != ret)
          {
             fprintf(stderr, "Fail to destroy handle : result(%d)", ret);
             return ret;
          }
        free(tts);
        tts = NULL;
     }
   return 1;
}

EAPI void
out_read(const char *txt)
{
   int ret = 0;
   int u_id = 0;
   ret = tts_add_text(tts, txt, "en_US", TTS_VOICE_TYPE_AUTO,
                      TTS_SPEED_NORMAL, &u_id);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to add text : ret(%d)\n", ret);
     }

   ret = tts_play(tts);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to play TTS : ret(%d)\n", ret);
     }
   return;
}


EAPI void
out_cancel(void)
{
   int ret = 0;
   if (tts)
     {
         ret = tts_stop(tts);
         if (TTS_ERROR_NONE != ret)
           {
             fprintf(stderr, "Fail to stop TTS : result(%d)", ret);
           }
     }
   return;
}

/*
 * unused api
 */

#if 0
EAPI void
out_read_done(void)
{
}

static void (*cb_func) (void *data);
static void *cb_data;

EAPI void
out_done_callback_set(void (*func) (void *data), const void *data)
{
   cb_func = func;
   cb_data = (void *)data;
}
#endif
