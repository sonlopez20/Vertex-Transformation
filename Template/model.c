
//----------------------------------------------------------
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "model.h"

//----------------------------------------------------------
Model *allocModel( int numVertices, int numFaces )
{
  // TODO: Allocate a Model structure.
  //       Initialize its fields using the given number of
  //         vertices and faces.
  //       Return a pointer to the Model structure as the
  //         result of this function.
  // (You can use calloc() to allocate zeroed out memory.  This
  //  is useful when allocating the vertex and face arrays.)
    
    Model *model;
    model = (Model *) calloc(1, sizeof(Model));
    
    if (model == NULL)
    {
        printf("Unable to allocate memory for Model.\n");
        exit(1);
    }
    if(numVertices > 0)
    {
        model->m_numVertices = numVertices;
        model->m_vertex = (Vertex *) calloc(numVertices, sizeof(Vertex));
        if(model->m_vertex == NULL)
        {
           printf("Unable to allocate memory for Vertices.\n");
        }
    }
    if(numFaces > 0)
    {
        model->m_numFaces = numFaces;
        model->m_face = (Face *) calloc(numFaces, sizeof(Face));
        if(model->m_face == NULL)
        {
           printf( "Unable to allocate memory for Faces.\n");
        }
    }
    return model;
}

//----------------------------------------------------------
void dumpModel( Model *model )
{
  printf( "#- Model parameters --------------------\n" );
  printf( "Vertex count : %5d\n", model->m_numVertices );
  printf( "Face count   : %5d\n", model->m_numFaces );

  printf( "\n#- Vertex list -------------------------\n" );
  for ( int i=0; i<model->m_numVertices; i++ ) {
    printf( "  [%5d]  ", i );
    dumpVertex( &model->m_vertex[i] );
    putc( '\n', stdout );
  }

  printf( "\n#- Face list ---------------------------\n" );
  for ( int i=0; i<model->m_numFaces; i++ ) {
    printf( "  [%5d]  ", i );
    dumpFace( &model->m_face[i] );
    putc( '\n', stdout );
  }

  printf( "#---------------------------------------\n" );
}

//----------------------------------------------------------
void freeModel( Model *model )
{
  // TODO: Free the vertex and face data of the given model.
  //       Free the model structure itself.
    if(model->m_vertex != NULL)
    {
        free(model->m_vertex);
    }
    if(model->m_face != NULL)
    {
        free(model->m_face);
    }
    free(model);
}

//----------------------------------------------------------
Model *loadModel( char *fName )
{
  // TODO: Open the file indicated by fName.
  //       Determine how many vertex lines and how many face
  //         lines it includes.
  //       Allocate a Model structure using the vertex and face
  //         counts as arguments.
  //       Read each vertex line from the file and save its X, Y,
  //         Z coordinates.
  //       Read each face line from the file and save its three
  //         vertex numbers.  Don't forget to subtract one from
  //         each number so it can be used as an index into the
  //         vertex list.
  //       Return a pointer to the allocated model structure as
  //         the value of this function.
    int numVertices = 0;
    int numFaces = 0;
    char *line = NULL;
    size_t lengthOfLine = 0;
    size_t readLine;
    
    FILE *fp = fopen(fName,"r");
    if (fp == NULL)
    {
        printf("File not opened\n");
        exit(1);
    }
    
    while ( ( readLine = getline( &line, &lengthOfLine, fp ) ) != -1 )
    {
       char *ch = line;

       while ( *ch && isspace( *ch ) )
       {
           ch++;
       }
       if ( *ch == 'v' )
       {
          ++numVertices;
       }
       if ( *ch == 'f' )
       {
          ++numFaces;
       }
     }

     rewind( fp );

     Model *model = allocModel( numVertices, numFaces );

     Vertex *vertex = model->m_vertex;
     Face *face = model->m_face;
     int lineNum = 0;

     while ( ( readLine = getline( &line, &lengthOfLine, fp ) ) != -1 )
     {
       lineNum++;

       char *ch = line;

       while ( *ch && isspace( *ch ) )
       {
           ch++;
       }

       if ( *ch == 'v' )
       {
         int convert = sscanf( ch+1, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z );
           
         if ( convert != 3 )
         {
           printf("Unable to load vertex.");
         }
         vertex++;
         continue;
       }

       if ( *ch == 'f' )
       {
         int convert = sscanf( ch+1, "%d %d %d", &face->v1, &face->v2, &face->v3 );

         face->v1--;
         face->v2--;
         face->v3--;

         if ( convert != 3 )
         {
           printf("Unable to load face");
         }
         face++;
         continue;
       }
     }
    free( line );
    fclose( fp );
    return model;
}
//----------------------------------------------------------
