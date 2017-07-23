#include "Icone.h"


void Icone::Init(){
    textureID = SOIL_load_OGL_texture(
                this->filepath.c_str(),
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_POWER_OF_TWO
                | SOIL_FLAG_MIPMAPS
                //| SOIL_FLAG_MULTIPLY_ALPHA
                //| SOIL_FLAG_COMPRESS_TO_DXT
                //| SOIL_FLAG_DDS_LOAD_DIRECT
                //| SOIL_FLAG_NTSC_SAFE_RGB
                //| SOIL_FLAG_CoCg_Y
                //| SOIL_FLAG_TEXTURE_RECTANGLE
                );
    visible=true;
    pose={0,0,1};
}

void Icone::Tick(const int64_t &milliseconds){

}

void Icone::Render(){
    if (visible){
        glBindTexture( GL_TEXTURE_2D, textureID );
        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
        glTranslated(pose.x,pose.y,0);
        glScalef(pose.scale,pose.scale,pose.scale);
        glColor4f( 1.0f, 1.0f, 1.0f, 0.5f );

        const float ref_mag = 0.1f;
        float tex_u_max = 1.0f;//0.2f;
        float tex_v_max = 1.0f;//0.2f;
        glBegin(GL_QUADS);
            glNormal3f( -ref_mag, -ref_mag, 1.0f );
            glTexCoord2f( 0.0f, tex_v_max );
            glVertex3f( -1.0f, -1.0f, -0.1f );

            glNormal3f( ref_mag, -ref_mag, 1.0f );
            glTexCoord2f( tex_u_max, tex_v_max );
            glVertex3f( 1.0f, -1.0f, -0.1f );

            glNormal3f( ref_mag, ref_mag, 1.0f );
            glTexCoord2f( tex_u_max, 0.0f );
            glVertex3f( 1.0f, 1.0f, -0.1f );

            glNormal3f( -ref_mag, ref_mag, 1.0f );
            glTexCoord2f( 0.0f, 0.0f );
            glVertex3f( -1.0f, 1.0f, -0.1f );
        glEnd();
        glPopMatrix();
    }
}
