#include "GraphicsPch.h"
#include "ASEParser/CASEParser.h"


CASEParser::CASEParser()
{
	m_materialcount = 0;
	m_parsingmode = eNone;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}
//---------------------------------------------------------------------------------------------------
// 로딩한다.
// 이것이 끝났다면 정해진 데이터형에 모든 데이터를 읽어서 들어가 있어야 한다.
//
// Parsing에 대해:
// 항상 느끼는 것이지만 parsing이라는 것은 데이터가 일정 규격에 맞게 들어가 있다는 것을 전제로 한다.
// 하지만, 파일 내부에 순서가 바뀌어 들어가 있는것이 가능하다던지 하는 규칙이 생기면
// 검색하는 루틴이 복잡해지기 마련. 일단은 순서대로 일정 규약으로 들어가 있다는것을 가정하자.
// -> 그래서, 재귀 호출을 하는 하나의 큰 함수로 해결봤다.
// -> depth를 기준으로 오동작에 대한 안전 코드를 넣어야겠다
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) 파일을 로드한다.
	if (!m_lexer->Open(p_File))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	/// 1) 다롱이아부지김교수의 파워풀한 재귀함수 한개로 처리 끝!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1차 변환

// CScenedata 복사
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// 값 복사
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// 메시의 최적화를 해 준다.
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고..
/// </summary>
bool CASEParser::Convert_Optimize(GeomObject* pGeomObj)
{
	// 다시 넣어보자.. x, y축을 외적하고. z축과 내적.
	XMVECTOR _cross = XMVector3Cross(pGeomObj->m_tm_row0, pGeomObj->m_tm_row2);
	if (XMVector3Dot(_cross, pGeomObj->m_tm_row1).m128_f32[0] < 0)
	{
		pGeomObj->m_tm_row0 *= -1;
		pGeomObj->m_tm_row1 *= -1;
		pGeomObj->m_tm_row2 *= -1;

		for (auto& value : pGeomObj->m_meshface)
		{
			value->m_normal *= -1;
			value->m_normalvertex[0] *= -1;
			value->m_normalvertex[1] *= -1;
			value->m_normalvertex[2] *= -1;
		}
	}

	// 스키닝 값 복사..!
	if (pGeomObj->m_is_skinningobject)
	{
		// 각 버텍스에 스키닝 관련 정보를 넣어준다?
		// WVertex는.. 쪼개기전 버텍스에 연결된 본의 인덱스와 가중치들이 들어가 있다..
	}

	// opt 버텍스에 일단 다 넣어주고 특정 상황에서 쪼개준다.
		// 내가 한거는 다 쪼갠 뒤 넣어준 거에 가깝다.
	// 버텍스 일단 복사
	for (int i = 0; i < pGeomObj->m_meshvertex.size(); i++)
	{
		// 새 버텍스를 만들어서 넣어준다.
		Vertex* _pVertex = new Vertex;

		// 파싱한 위치값은 그대로 넣어준다.
		_pVertex->m_pos = pGeomObj->m_meshvertex[i]->m_pos;

		// 각종 값들을 넣어준다.
		_pVertex->m_normal = pGeomObj->m_meshvertex[i]->m_normal;
		// 기본적으로 현재는 노말이 세팅되어 있지 않다.
		_pVertex->m_isnormalset = pGeomObj->m_meshvertex[i]->m_isnormalset;

		// 텍스쳐 좌표도 넣어준다.
		_pVertex->u = pGeomObj->m_meshvertex[i]->u;
		_pVertex->v = pGeomObj->m_meshvertex[i]->v;
		_pVertex->m_istextureset = pGeomObj->m_meshvertex[i]->m_istextureset;

		// 스킨드를 위한 각종 정보들을 넣어준다.
		_pVertex->m_bw1 = pGeomObj->m_meshvertex[i]->m_bw1;
		_pVertex->m_bw2 = pGeomObj->m_meshvertex[i]->m_bw2;
		_pVertex->m_bw3 = pGeomObj->m_meshvertex[i]->m_bw3;

		// 인덱스 정보를 넣어준다. => 해당 인덱스 번호로 Wvertex와 연결해줄 수 있을 듯 하다.
		_pVertex->m_indices = pGeomObj->m_meshvertex[i]->m_indices;

		// 그리고 일단 해당 정보들은 넣어준다.
		pGeomObj->m_opt_vertex.push_back(_pVertex);
	}

	// face를 돌면서 노멀값을 세팅해준다.
		// 이 부분을 지나면, 포지션과 노말이 세팅되어 있는 친구가 나온다.
	for (int i = 0; i < pGeomObj->m_meshface.size(); i++)
	{
		// 각 페이스 별 인덱스 세 개를 돌면서.. 노말값이 세팅되어 있지 않으면 세팅해준다.
		for (int j = 0; j < 3; j++)
		{
			// 버텍스 하나하나를 보면서 opt_vertex에 정보를 갱신해준다.
				// i번째 면의 j번째 인덱스의 버텍스 값을 가져온다.
			Vertex* _nowVertex = pGeomObj->m_opt_vertex[pGeomObj->m_meshface[i]->m_vertexindex[j]];

			// 노말값이 세팅되어 있는가.
				// 기본값이 false기 때문에 여기서 세팅해준다.
			if (!_nowVertex->m_isnormalset)
			{
				// normal face에 추가해준 normal 값을 넣어줍니다.
				_nowVertex->m_normal = pGeomObj->m_meshface[i]->m_normalvertex[j];
				// 그리고 현재 버텍스의 노말값이 세팅되었다고 해줍니다.
				_nowVertex->m_isnormalset = true;
			}
			// 세팅이 되어있을 때, 새로 추가 해준다 = convert all과 같은방식
				// 노말 값을 비교해서 다르면 추가해준다.
			else
			{
				// 노말 값이 한번은 지정되어 있다.
					// 현재의 노말 값과 다를 때만 세팅한다.
				if (_nowVertex->m_normal != pGeomObj->m_meshface[i]->m_normalvertex[j])
				{
					// 노말 값이 다르면 새 버텍스를 만들어서 넣어주자.
					Vertex* _newVertex = new Vertex();

					// 포지션은 가져와 줄 수 있다.
					int vertexIndex = pGeomObj->m_meshface[i]->m_vertexindex[j];
					_newVertex->m_pos = pGeomObj->m_opt_vertex[vertexIndex]->m_pos;

					// 새 노말 값을 넣어줍니다.
					_newVertex->m_normal = pGeomObj->m_meshface[i]->m_normalvertex[j];
					// 노말 값을 넣어줬기에 세팅되었다고 해줍니다.
					_newVertex->m_isnormalset = true;

					// 버텍스를 쪼개 줄 때 같은 인덱스 번호를 사용합니다. (포지션이 같은 위치에 있는 버텍스기 때문)
						// 그림 그릴 때 사용하는 인덱스와 다른 인덱스입니다.
					_newVertex->m_indices = _nowVertex->m_indices;

					// 새 버텍스를 넣어줍니다.
					pGeomObj->m_opt_vertex.push_back(_newVertex);

					// 페이스의 index값들도 변경해줍니다.
					pGeomObj->m_meshface[i]->m_vertexindex[j] = pGeomObj->m_opt_vertex.size() - 1;
				}
			}
		}
	}

	// 노말에서 split을 했기 때문에.. 모든 면을 돌면서 Texture 값을 넣어본다.
	for (int i = 0; i < pGeomObj->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// 버텍스 하나하나를 보면서 opt_vertex에 정보를 갱신해준다.
			Vertex* _nowVertex = pGeomObj->m_opt_vertex[pGeomObj->m_meshface[i]->m_vertexindex[j]];

			// 텍스쳐 값이 없으면 굳이 정보를 갱신해줄 필요가 없기 때문에 break;
			if (pGeomObj->m_mesh_numtvertex == 0)
				break;

			// 텍스쳐 값이 설정되어 있는가.
			if (!_nowVertex->m_istextureset)
			{
				// 해당 페이스의 인덱스 값을 가져온다..
				_nowVertex->u = pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_u;
				_nowVertex->v = pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_v;

				_nowVertex->m_istextureset = true;
			}
			// 세팅이 되어있을 때..
				// 텍스쳐 값이 다를 때만 세팅한다.
				// 이 때의 노말값은 어떻게..
			else
			{
				if ((_nowVertex->u != pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_u)
					||
					(_nowVertex->v != pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_v))
				{
					// 그 지점의 노말 값을 가져온다..?
					Vertex* _newVertex = new Vertex();

					// 포지션, 노말은 같다고 가정.
					_newVertex->m_pos = pGeomObj->m_opt_vertex[pGeomObj->m_meshface[i]->m_vertexindex[j]]->m_pos;
					_newVertex->m_normal = pGeomObj->m_meshface[i]->m_normalvertex[j];
					_newVertex->m_isnormalset = true;

					// 텍스쳐를 넣어줍니다.
					_newVertex->u = pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_u;
					_newVertex->v = pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_v;

					// 버텍스를 쪼개 줄 때 같은 인덱스 번호를 사용합니다. (포지션이 같은 위치에 있는 버텍스기 때문)
						// 그림 그릴 때 사용하는 인덱스와 다른 인덱스입니다.
					_newVertex->m_indices = _nowVertex->m_indices;

					// 새 버텍스를 넣고 인덱스 값을 변경해줍니다.
					pGeomObj->m_opt_vertex.push_back(_newVertex);
					pGeomObj->m_meshface[i]->m_vertexindex[j] = pGeomObj->m_opt_vertex.size() - 1;
				}
			}
		}
	}

	// 일단 여기서 opt_index를 한 번 만들어주자(position만 가지고 있는 index 구조)
	pGeomObj->m_opt_index = new IndexList[pGeomObj->m_meshface.size()];

	for (unsigned int i = 0; i < pGeomObj->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pGeomObj->m_opt_index[i].index[j] = pGeomObj->m_meshface[i]->m_vertexindex[j];
		}
	}
	// 여기까지 오면 성공
	return true;
}

bool CASEParser::Convert_OptimizeLight(GeomObject* pGeomObj)
{
	// 다시 넣어보자.. x, y축을 외적하고. z축과 내적. => 함수 제대로 봅시다..
	XMVECTOR _cross = XMVector3Cross(pGeomObj->m_tm_row0, pGeomObj->m_tm_row2);
	if (XMVector3Dot(_cross, pGeomObj->m_tm_row1).m128_f32[0] < 0)
	{
		pGeomObj->m_tm_row0 *= -1;
		pGeomObj->m_tm_row1 *= -1;
		pGeomObj->m_tm_row2 *= -1;

		for (auto& value : pGeomObj->m_meshface)
		{
			value->m_normal *= -1;
			value->m_normalvertex[0] *= -1;
			value->m_normalvertex[1] *= -1;
			value->m_normalvertex[2] *= -1;
		}
	}

	// opt 버텍스에 일단 다 넣어주고 특정 상황에서 쪼개준다.
		// 내가 한거는 다 쪼갠 뒤 넣어준 거에 가깝다.
	// 버텍스 일단 복사
	for (int i = 0; i < pGeomObj->m_meshvertex.size(); i++)
	{
		// 새 버텍스를 만들어서 넣어준다.
		Vertex* _pVertex = new Vertex;

		// 파싱한 위치값은 그대로 넣어준다.
		_pVertex->m_pos = pGeomObj->m_meshvertex[i]->m_pos;

		// 각종 값들을 넣어준다.
		_pVertex->m_normal = pGeomObj->m_meshvertex[i]->m_normal;
		// 기본적으로 현재는 노말이 세팅되어 있지 않다.
		_pVertex->m_isnormalset = pGeomObj->m_meshvertex[i]->m_isnormalset;

		// 텍스쳐 좌표도 넣어준다.
		_pVertex->u = pGeomObj->m_meshvertex[i]->u;
		_pVertex->v = pGeomObj->m_meshvertex[i]->v;
		_pVertex->m_istextureset = pGeomObj->m_meshvertex[i]->m_istextureset;

		// 스킨드를 위한 각종 정보들을 넣어준다.
		_pVertex->m_bw1 = pGeomObj->m_meshvertex[i]->m_bw1;
		_pVertex->m_bw2 = pGeomObj->m_meshvertex[i]->m_bw2;
		_pVertex->m_bw3 = pGeomObj->m_meshvertex[i]->m_bw3;

		// 인덱스 정보를 넣어준다.
		_pVertex->m_indices = pGeomObj->m_meshvertex[i]->m_indices;

		// 그리고 일단 해당 정보들은 넣어준다.
		pGeomObj->m_opt_vertex.push_back(_pVertex);
	}

	// face를 돌면서 노멀값을 세팅해준다.
		// 이 부분을 지나면, 포지션과 노말이 세팅되어 있는 친구가 나온다.
	for (int i = 0; i < pGeomObj->m_meshface.size(); i++)
	{
		// 각 페이스 별 인덱스 세 개를 돌면서.. 노말값이 세팅되어 있지 않으면 세팅해준다.
		for (int j = 0; j < 3; j++)
		{
			// 버텍스 하나하나를 보면서 opt_vertex에 정보를 갱신해준다.
				// i번째 면의 j번째 인덱스의 버텍스 값을 가져온다.
			Vertex* _nowVertex = pGeomObj->m_opt_vertex[pGeomObj->m_meshface[i]->m_vertexindex[j]];

			// 노말값이 세팅되어 있는가.
				// 기본값이 false기 때문에 여기서 세팅해준다.
			if (!_nowVertex->m_isnormalset)
			{
				// normal face에 추가해준 normal 값을 넣어줍니다.
				_nowVertex->m_normal = pGeomObj->m_meshface[i]->m_normalvertex[j];
				// 그리고 현재 버텍스의 노말값이 세팅되었다고 해줍니다.
				_nowVertex->m_isnormalset = true;
				// 텍스쳐가 없기 때문에 버텍스에 임의의 텍스쳐 값을 넣어줍니다.
				_nowVertex->u = 0.0f;
				_nowVertex->v = 0.0f;
				_nowVertex->m_istextureset = true;

			}
			// 세팅이 되어있을 때, 새로 추가 해준다 = convert all과 같은방식
				// 노말 값을 비교해서 다르면 추가해준다.
			else
			{
				// 노말 값이 한번은 지정되어 있다.
					// 현재의 노말 값과 다를 때만 세팅한다.
				if (_nowVertex->m_normal != pGeomObj->m_meshface[i]->m_normalvertex[j])
				{
					// 노말 값이 다르면 새 버텍스를 만들어서 넣어주자.
					Vertex* _newVertex = new Vertex();

					// 포지션은 가져와 줄 수 있다.
					int vertexIndex = pGeomObj->m_meshface[i]->m_vertexindex[j];
					_newVertex->m_pos = pGeomObj->m_opt_vertex[vertexIndex]->m_pos;

					// 새 노말 값을 넣어줍니다.
					_newVertex->m_normal = pGeomObj->m_meshface[i]->m_normalvertex[j];
					// 노말 값을 넣어줬기에 세팅되었다고 해줍니다.
					_newVertex->m_isnormalset = true;

					_newVertex->u = 0.0f;
					_newVertex->v = 0.0f;
					_newVertex->m_istextureset = true;

					// 새 버텍스를 넣어줍니다.
					pGeomObj->m_opt_vertex.push_back(_newVertex);

					// 인덱스 정보를 갱신해줍니다.
					//pMesh->m_opt_index[i].index[j] = pMesh->m_opt_vertex.size() - 1;

					// 페이스의 index값들도 변경해줍니다.
					pGeomObj->m_meshface[i]->m_vertexindex[j] = pGeomObj->m_opt_vertex.size() - 1;
				}
			}
		}
	}

	// 일단 여기서 opt_index를 한 번 만들어주자(position만 가지고 있는 index 구조)
	pGeomObj->m_opt_index = new IndexList[pGeomObj->m_meshface.size()];

	for (unsigned int i = 0; i < pGeomObj->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pGeomObj->m_opt_index[i].index[j] = pGeomObj->m_meshface[i]->m_vertexindex[j];
		}
	}
	// 여기까지 오면 성공
	return true;
}

// 내가 만든 ConvertAll
	// 중복 신경 안쓰고 일단 다 쪼개주는 함수..
bool CASEParser::ConvertAll(GeomObject* pMesh)
{
	// 우선 특정 위치의 버텍스를 임시로 복사 => 해당 벡터의 포지션을 사용한다.
	std::vector<Vertex*> _posVertex;

	for (int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* _pVertex = new Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;

		_posVertex.push_back(_pVertex);
	}

	// Light까지만..
		// 이후에 텍스쳐는 합쳐지는 연산이 들어간다 어떻게 해결 할 수 있을까..
	// 현재 인덱스는 그려지는 순서대로 들어가 있다..
		// 노말의 경우 ABC가 아닌 ACB로 들어가 있을 것.. => 그대로 넣었기 때문
		// 이 부분을 바꿔주고 인덱스를 순서대로 넣어준다.
		// 버텍스에 노말 값을 넣어주고 인덱스에는 인덱스 카운트를 순서대로 넣어준다..
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	// 나머지는 face를 기준으로 한 인덱스로 찾아야 함
		// 페이스 개수 * 3으로 돌면서 추가해준다.

	/*unsigned int _faceCount = pMesh->m_meshface.size();
	TRACE("FaceCount : %d", _faceCount);*/

	int indexCount = 0;

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		//TRACE("FaceNum : %d / %d\n", i, _faceCount);
		for (int j = 0; j < 3; j++)
		{
			// 새 버텍스를 만들어서 opt_vertex에 넣어준다.
			Vertex* _pVertex = new Vertex;
			// 포지션은 해당 인덱스에 있는 버텍스의 포지션.
			_pVertex->m_pos = _posVertex[pMesh->m_meshface[i]->m_vertexindex[j]]->m_pos;

			// 노말 값은, 해당 메시 페이스의 해당하는 버텍스의 노말값.
				// 인데 j가 1이면 2의 값을, 2면 1의 값을 넣어준다.
			_pVertex->m_normal = pMesh->m_meshface[i]->m_normalvertex[j];

			// 새로 만든 버텍스를 넣어주고..
			pMesh->m_opt_vertex.push_back(_pVertex);

			// 인덱스카운트를 늘리면서 넣어준다.
				// 새로만든 버텍스들은 중복없이 0, 1, 2, 3, 4, 5 등의 순으로 들어가게 될 것.
			pMesh->m_opt_index[i].index[j] = indexCount++;
		}
	}


	// 끝나고 임시 객체의 메모리를 해제해 준다
	for (auto i : _posVertex)
	{
		delete i;
	}

	return true;
}


ASEParser::GeomObject* CASEParser::GetGeomObj(int index)
{
	return m_GeomObjList[index];
}

//----------------------------------------------------------------
// 재귀 호출됨을 전제로 하는 분기 함수이다.
//
// 일단 읽고, 토큰에 따라 동작한다.
//
// 재귀를 들어가는 조건은 { 을 만날 경우이다.
// 리턴하는 조건은 '}'를 만나거나 TOKEND_END를 만났을 때이다.
//
// 더불어, 재귀를 들어가기 전 토큰을 읽어, 지금 이 함수가 어떤 모드인지를 결정한다.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// 지역변수들 선언
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;
	// 리스트에 데이터를 넣어야 할 때 쓰이는 리스트 카운터.
	int listcount = 0;		// 매우 C스러운데 다른 해결책이 없을까?
	// 이것은 현 토큰이 어떤 종류인가를 판별해준다.
	// 이것을 멤버 변수로 하고 재귀를 돌리는것은 위험하다? -> 잘못된 재귀방식이다?
	LONG nowtoken;
	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	// 노말 값을 위한.. 페이스 정보(이 부분 한 번 갈아야 할 거 같은데..)
	int _tempFace = 0;


	/// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면.
		// 넘어온 토큰에 따라 처리해준다.

		static int iv = 0;

		switch (nowtoken)
		{
		case TOKEND_BLOCK_START:

			Parsing_DivergeRecursiveALL(depth++);
			break;

			//--------------------
			// 3DSMAX_ASCIIEXPORT
			//--------------------

		case TOKENR_3DSMAX_ASCIIEXPORT:
			m_data_asciiexport = Parsing_NumberLong();
			break;

			//--------------------
			// COMMENT
			//--------------------

		case TOKENR_COMMENT:
			Parsing_String();	// 그냥 m_TokenString에 읽어버리는 역할 뿐.
			//AfxMessageBox( m_TokenString, NULL, NULL);		/// 임시로 코멘트를 출력해본다
			break;

			//--------------------
			// SCENE
			//--------------------

		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_filename = Parsing_String();		// 일관성 있는 함수의 사용을 위해 String과 Int도 만들어줬다.
			break;
		case TOKENR_SCENE_FIRSTFRAME:
			m_scenedata.m_firstframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_LASTFRAME:
			m_scenedata.m_lastframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_FRAMESPEED:
			m_scenedata.m_framespeed = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_TICKSPERFRAME:
			m_scenedata.m_ticksperframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_MESHFRAMESTEP:
			m_scenedata.m_meshframestep = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_KEYFRAMESTEP:
			m_scenedata.m_keyframestep = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_BACKGROUND_STATIC:
			m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
			break;
		case TOKENR_SCENE_AMBIENT_STATIC:
			m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
			break;

		case TOKENR_SCENE_ENVMAP:
		{
			// 그냥 안의 내용을 읽어버린다 }가 나올때까지
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
			}
		}
		break;

		//--------------------
		// MATERIAL_LIST
		//--------------------

		case TOKENR_MATERIAL_COUNT:
		{
			m_materialcount = Parsing_NumberInt();
		}
		break;

		// 머테리얼을 만나면 새 머테리얼을 하나 만들어서 머테리얼 리스트에 넣는다.
		case TOKENR_MATERIAL:
		{
			// 머테리얼을 하나 만들어서 리스트에 넣는다.
			Create_materialdata_to_list();

			m_materialdata->m_materialnumber = Parsing_NumberInt();
		}
		break;

		// 각종 값들을 파싱해줍시다.
		case TOKENR_MATERIAL_NAME:
		{
			m_materialdata->m_material_name = Parsing_String();
		}
		break;
		case TOKENR_MATERIAL_CLASS:
		{
			m_materialdata->m_material_class = Parsing_String();
		}
		break;
		case TOKENR_MATERIAL_AMBIENT:
		{
			m_materialdata->m_material_ambient = Parsing_NumberVector3Mat();
		}
		break;
		case TOKENR_MATERIAL_DIFFUSE:
		{
			m_materialdata->m_material_diffuse = Parsing_NumberVector3Mat();
		}
		break;
		case TOKENR_MATERIAL_SPECULAR:
		{
			m_materialdata->m_material_specular = Parsing_NumberVector3Mat();
		}
		break;
		case TOKENR_MATERIAL_SHINE:
		{
			m_materialdata->m_material_shine = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_SHINESTRENGTH:
		{
			m_materialdata->m_material_shinestrength = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_TRANSPARENCY:
		{
			m_materialdata->m_material_transparency = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_WIRESIZE:
		{
			m_materialdata->m_material_wiresize = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_SHADING:
		{
			// ASE에서는 Blinn과 같은 값으로 나와 있다.
			m_materialdata->m_material_shading = Parsing_NumberInt();
		}
		break;
		case TOKENR_MATERIAL_XP_FALLOFF:
		{
			m_materialdata->m_material_xp_falloff = Parsing_NumberFloat();
		}
		break;

		// 디퓨즈 맵을 만난다면?
		case TOKENR_MAP_DIFFUSE:
		{
			// 새 맵을 만들어서 추가해준다.
			MaterialMap* _tempMap = new MaterialMap();

			m_materialdata->m_map_diffuse = _tempMap;
			m_parsingmode = eParsingmode::eDiffuseMap;
		}
		break;
		// 맵이름 추가
		case TOKENR_MAP_NAME:
		{
			m_materialdata->m_map_diffuse->m_map_name = Parsing_String();
		}
		break;
		case TOKENR_MAP_CLASS:
		{
			m_materialdata->m_map_diffuse->m_map_class = Parsing_String();
		}
		break;
		case TOKENR_MAP_SUBNO:
		{
			m_materialdata->m_map_diffuse->m_subno = Parsing_NumberInt();
		}
		break;
		case TOKENR_MAP_AMOUNT:
		{
			m_materialdata->m_map_diffuse->m_map_amount = Parsing_NumberFloat();
		}
		break;
		// 일단은 가장 핵심적인 비트맵까지만..(텍스쳐 위치 파악)
		case TOKENR_BITMAP:
		{
			if (m_parsingmode == eDiffuseMap)
			{
				m_materialdata->m_map_diffuse->m_bitmap = Parsing_String();
			}
		}
		break;
		// 사실 가능하면 타일맵이나 UV offset도 처리해주면 좋긴하지만.. 일단은 비트맵 까지만 한 뒤 node로 넘어가겠습니다..
		// 겐지오류를 막기위해
		case TOKENR_MAP_BUMP:
		{
			// BitMap 중복을 막기 위한 파싱모드 변경
			m_parsingmode = eParsingmode::eNone;
		}
		break;

		//--------------------
		// GEOMOBJECT
		//--------------------

		case TOKENR_GROUP:
			//	한 개의 그룹 시작. 이 다음에 이름이 스트링으로 나오기는 하는데.
			break;

		case TOKENR_HELPEROBJECT:
			// 일단 생성하고
			// 오브젝트의 타입 정해줌. 이것에 따라 서로 다른 파싱 모드 발동.
		{
			// Mesh는 없지만 정보는 가지고 있어야하기 때문에..
			Create_onemesh_to_list();

			m_parsingmode = eParsingmode::eHelperObject;
			// 헬퍼오브젝트는 Mesh가 존재하지 않기 때문에 관련된 부분을 0으로 해준다.
			m_TempGeomObject->m_mesh_numvertex = 0;
			m_TempGeomObject->m_mesh_numfaces = 0;
		}
		break;

		case TOKENR_GEOMOBJECT:
		{
			/// 이 토큰을 만났다는건 새로운 메시가 생겼다는 것이다. 지역 변수로 mesh를 하나 선언, 그 포인터를 리스트에 넣고, m_onemesh에 그 포인터를 복사, 그대로 쓰면 될까?

			Create_onemesh_to_list();

			// 파싱모드 설정.
			m_parsingmode = eParsingmode::eGeomobject;
		}
		break;

		case TOKENR_NODE_NAME:
			// 어쩄든 지금은 오브젝트들을 구별 할 수 있는 유일한 값이다.
			// 모드에 따라 넣어야 할 곳이 다르다.
		{
			// 일단 지옴 오브젝트 일때부터.. 이후에 다른 애들은 다르게 넣어주면 되지 않을까.
			switch (m_parsingmode)
			{
			case eParsingmode::eGeomobject:
			{
				// 현재 파싱하는 메시에 이름을 넣어준다.
				// 리스트에 들어가 있는 포인터 값과 지금 one_mesh에 들어가 있는 포인터 값이 같기 때문에
				// one_mesh만 교쳐도 리스트에 들어가 있는 메시의 값도 바뀔 것..
				m_TempGeomObject->m_nodename = Parsing_String();
			}
			break;
			case eParsingmode::eHelperObject:
			{
				// NODE_TM에서도 하나 나오긴 하지만, 대부분 동일하다 가정하고..
				m_TempGeomObject->m_nodename = Parsing_String();
			}
			break;
			case eShape:
			{
				m_TempGeomObject->m_nodename = Parsing_String();
			}
			break;
			case eAnimation:
			{
				// 애니메이션이면..
				m_animation->m_nodename = Parsing_String();
			}
			break;
			default:

				break;
			}
		}
		break;

		case TOKENR_HELPER_CLASS:
		{
			// 헬퍼 클래스 확인..
			if (m_parsingmode == eHelperObject)
			{

			}
		}
		break;

		case TOKENR_NODE_PARENT:
			// 현 노드의 부모 노드의 정보.
			// 일단 입력을 하고, 나중에 정리하자.
			// 모드를 잘 설정해주자.
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eShape || m_parsingmode == eHelperObject)
			{
				m_TempGeomObject->m_nodeparent = Parsing_String();
				m_TempGeomObject->m_isparentexist = true;
			}
		}
		break;

		/// NODE_TM

		case TOKENR_NODE_TM:
			//m_parsingmode	=	eGeomobject;
			// (NODE_TM으로 진입 후 NODE_NAME이 한번 더 나온다.)
			// (Animation과도 구별을 해야 하기 때문에 이렇게 모드를 적어준다)

			/// 게다가,
			// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
			// 아예 이 재귀에서 리턴을 시키고 싶지만. 중간에 읽는것을 무시해야 하기 때문에...
			//if (m_onemesh->m_camera_isloadTarget) {
			//	'}'이 나올때까지 나오는건 무조건 무시! 뭐 이런거 안되나..
			// 재귀구조의 문제점이다....

			// 일단 지금은 넘어가자.. Node때 머리 터지겠군...
				// 따로 파싱 모드를 만들어주지 않기 때문에 + nodeName이 같기 때문에 그냥 넘어가 준다..
			break;

		case TOKENR_INHERIT_POS:
			// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_inherit_pos = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_INHERIT_ROT:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_inherit_rot = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_INHERIT_SCL:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_inherit_scl = Parsing_NumberVector3();
			}
		}
		break;

		// 축의 방향, 일단 지금은 읽고 나중에 불러 올 때 row1, 2를 변환 해준다.
			// 지금은 col을 변환한다 생각하자
		case TOKENR_TM_ROW0:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_row0 = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_ROW1:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_row1 = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_ROW2:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_row2 = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_ROW3:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_row3 = Parsing_NumberVector3();
			}
		}
		break;

		// 각각이 분리된 값들
			// 여기도 y, z를 바꿔준다..
		case TOKENR_TM_POS:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_pos = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_ROTAXIS:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_rotaxis = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_ROTANGLE:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_rotangle = Parsing_NumberFloat();
			}
		}
		break;
		case TOKENR_TM_SCALE:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_scale = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_SCALEAXIS:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_scaleaxis = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_SCALEAXISANG:
			// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_scaleaxisang = Parsing_NumberFloat();
			}
		}
		break;

		/// ANIMATION
		case TOKENR_TM_ANIMATION:
		{
			// 애니메이션 토큰을 만나면 애니메이션을 하나 넣어준다.
			Create_animationdata_to_list();
			m_parsingmode = eAnimation;

			m_TempGeomObject->m_isAnimated = true;
			m_animation->m_ticksperFrame = m_scenedata.m_ticksperframe;
		}
		break;
		// 포지션을 넣어보자..

		case TOKENR_CONTROL_POS_SAMPLE:
		{
			// 핵심은 샘플에 담겨있는 값..
			// 포지션을 만들어서 넣어준다.
			CAnimation_pos* _tempPos = new CAnimation_pos;
			_tempPos->m_time = Parsing_NumberInt();
			// y z 축을 조정.
			_tempPos->m_pos = Parsing_NumberVector3();
			m_animation->m_position.push_back(_tempPos);
		}
		break;
		// 로테이션을 넣어보자..

		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			CAnimation_rot* _tempRot = new CAnimation_rot;
			_tempRot->m_time = Parsing_NumberInt();
			_tempRot->m_rot = Parsing_NumberVector3();
			_tempRot->m_angle = Parsing_NumberFloat();

			// 쿼터니언은 어떻게 넣어주지.. => 애니메이션시에 angle을 가지고 누산해서 만들어준다.
				// 오일러각의 쿼터니언 변환이 필수적.. 변환후 넣어주고 해당 값을 애니메이션에서 사용한다..?

				// 쿼터니언을 만들어주고 => 승수형 코드에서 힌트.
			tempQT = XMQuaternionRotationAxis(
				{ _tempRot->m_rot.x, _tempRot->m_rot.y, _tempRot->m_rot.z }
			, _tempRot->m_angle);

			// 이전 값이 있으면 누적한다..?
			if (m_animation->m_rotation.size() == 0)
			{
				_tempRot->m_rotQT_accumulation = tempQT;
			}
			else
			{
				// 이전 쿼터니언을 가져와서 곱해준다.
				prevQT = m_animation->m_rotation.back()->m_rotQT_accumulation;
				_tempRot->m_rotQT_accumulation = prevQT * tempQT;
			}

			m_animation->m_rotation.push_back(_tempRot);
		}
		break; // 브레이크좀..
	// 스케일은 없지만 만들어만 두자.
		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			CAnimation_scl* _tempScl = new CAnimation_scl;
			_tempScl->m_time = Parsing_NumberInt();
			_tempScl->m_scale = Parsing_NumberVector3();
			_tempScl->m_scaleaxis = Parsing_NumberVector3();
			_tempScl->m_scaleaxisang = Parsing_NumberFloat();

			m_animation->m_scale.push_back(_tempScl);
		}
		break;

		/// MESH

		case TOKENR_MESH:
			//
			break;
		case TOKENR_TIMEVALUE:
		{
			m_TempGeomObject->m_timevalue = static_cast<float>(Parsing_NumberInt());
		}
		break;
		case TOKENR_MESH_NUMBONE:
			// 이게 있다면 이것은 Skinned Mesh라고 단정을 짓는다.
		{
			// Skining..
			// GeomObj에 정보들이 들어가 있다.
			m_TempGeomObject->m_is_skinningobject = true;
		}
		break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			break;
			// ASE에 있는 개수를 넣어주자.
		case TOKENR_MESH_NUMVERTEX:
			m_TempGeomObject->m_mesh_numvertex = Parsing_NumberInt();
			break;
		case TOKENR_MESH_NUMFACES:
			m_TempGeomObject->m_mesh_numfaces = Parsing_NumberInt();
			break;

			/// MESH_VERTEX_LIST

		case TOKENR_MESH_VERTEX_LIST:
			//
			// 버텍스의 값들을 집어넣어야 하는데
			// 이미 벡터로 선언이 돼 있으므로 그냥 넣으면 된다.
			// 버텍스 리스트에 넣어주기. Position에 따른 인덱스의 값과 위치 값이 들어간다.
		{
			// 리스트에는 어떤 정보가 있는가..
				// 내부적으로 바로 버텍스 토큰이 있어서 여기서 어떤 작업을 해줘야할지 모르겠다..
		}
		break;
		case TOKENR_MESH_VERTEX:
			// 데이터 입력
				// 실제 버텍스 정보가 들어가 있다..
		{
			// 구조 순서대로 넣어준다..?
			// 버텍스를 하나 생성해서 넣어주자.
			Vertex* _temp = new Vertex();

			_temp->m_indices = Parsing_NumberInt();
			// y와 z를 바꿔서 넣어준다.
			/*
			temp->m_pos.x = Parsing_NumberFloat();
			temp->m_pos.z = Parsing_NumberFloat();
			temp->m_pos.y = Parsing_NumberFloat();
			 */

			 // 이미 함수가 있다..
			_temp->m_pos = Parsing_NumberVector3();

			m_TempGeomObject->m_meshvertex.push_back(_temp);
		}
		break;

		/// Bone

		case TOKENR_SKIN_INITTM:
			break;
		case TOKENR_BONE_LIST:
		{
			m_parsingmode = eBone_List;
		}
		break;
		case TOKENR_BONE:
		{
			/// 모드 체인지 해 주고, Bone을 소유하고 있다는 것은 이것은 스키닝 오브젝트라는 것이다.
			// 본 하나를 만들어서 임시 포인터 보관, 벡터에 넣고
			// Bone의 넘버를 읽어 주자
			if (m_parsingmode = eBone_List)
			{
				Bone* _tempBone = new Bone;
				_tempBone->m_bone_number = Parsing_NumberInt();
				m_TempGeomObject->m_bone = _tempBone;
				m_TempGeomObject->m_vector_bone_list.push_back(_tempBone);
			}
		}
		break;
		//이 다음에 본의 이름을 넣어야 한다. 하지만 {를 한 개 더 열었으므로 임시 포인터 변수로서 보관해야겠지.
		case TOKENR_BONE_NAME:
		{
			m_TempGeomObject->m_bone->m_bone_name = Parsing_String();
		}
		break;
		case TOKENR_BONE_PROPERTY:
			// 이 다음 ABSOLUTE가 나오기는 하는데, 쓸 일이 없다.
			break;
			// 다음에는 TM_ROW0~3이 나오는데 역시 무시됨..

		case TOKENR_MESH_WVERTEXS:
			// 따로 해줄 일이 없다.
			break;

		case TOKENR_MESH_WEIGHT:
		{
			// 버텍스 하나의 정보를 만들어서 리스트에 넣음
				// 각각의 버텍스는 버텍스를 쪼개기 전의 버텍스 값과 완전히 일치한다
				// => 여기서 쪼개진 버텍스들도 같은 Weight를 가지고 있어야한다.
			VertexWeight* _tempWVertex = new VertexWeight;
			m_TempGeomObject->m_wvertex = _tempWVertex;
			_tempWVertex->m_wvertex_number = Parsing_NumberInt();
			m_TempGeomObject->m_vector_wvertexs.push_back(_tempWVertex);
		}
		break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			// 대체 몇 단계를 들어가는거야...
			// 가중치 한개를 만들어서 리스트에 넣는다
			/// 헥 헥....
			Weight* _tempBoneWeight = new Weight;
			_tempBoneWeight->m_bone_number = Parsing_NumberInt();
			_tempBoneWeight->m_bone_weight = Parsing_NumberFloat();
			m_TempGeomObject->m_wvertex->m_temp_bone_blend_weight = _tempBoneWeight;

			// 현재 파싱하고 있는 버텍스의 리스트에 넣어준다.
			m_TempGeomObject->m_wvertex->m_bone_blending_weight.push_back(_tempBoneWeight);
		}
		break;


		/// MESH_FACE_LIST
		case TOKENR_MESH_FACE_LIST:
			//
			break;
		case TOKENR_MESH_FACE:
		{
			// Face의 번호인데...

			// A:를 읽고
			// B:
			// C:

			/// (뒤에 정보가 더 있지만 default에 의해 스킵될 것이다.)
			/// ......

			// 벡터에 넣어준다.
				// 앞의 버텍스와 같은 방식으로 넣어준다. 대신 이때 인덱스 순서를 ABC가 아닌 반시계를 고려해주자..
				// A: 과 같은 것을 바꿔줘야..

			Face* _temp = new Face();
			// temp에 값들을 넣어줘야 하는데..
			// 각 값들 중 무시할 것들을 무시해줄 수 있나..
				// 첫번째 인덱스 번호는 무시..
			Parsing_NumberInt();
			Parsing_String();
			// 두 세번째 값을 가져온다..
				// 는 일단 load Goem에서 고쳐줄 거라 믿고 그냥 넣어주자.
			_temp->m_vertexindex[0] = Parsing_NumberInt();
			Parsing_String();
			_temp->m_vertexindex[1] = Parsing_NumberInt();
			Parsing_String();
			_temp->m_vertexindex[2] = Parsing_NumberInt();

			// 그리고 위치 변경..

			// 리스트에 넣기
			m_TempGeomObject->m_meshface.push_back(_temp);
		}
		break;

		// Normal
		case TOKENR_MESH_FACENORMAL:
		{
			// 같은 인덱스의 페이스에 페이스 노말을 넣어준다.
			int _tempIdx = Parsing_NumberInt();
			m_TempGeomObject->m_meshface[_tempIdx]->m_normal = Parsing_NumberVector3();
		}
		break;

		// 버텍스 노말.. => 해당 버텍스 인덱스의 포지션을 가져온 새로운 버텍스를 만들어서 넣는다..
		case TOKENR_MESH_VERTEXNORMAL:
		{
			// 첫번째 버텍스 인덱스를 넘겨주기 위해 파싱해준다.
			Parsing_NumberInt();

			int _tempFaceIdx = _tempFace / 3;
			int _normalVertexIdx = _tempFace % 3;

			m_TempGeomObject->m_meshface[_tempFaceIdx]->m_normalvertex[_normalVertexIdx] = Parsing_NumberVector3();

			_tempFace++;
		}
		break;

		case TOKENR_MESH_NUMTVERTEX:
		{
			// TVertex의 개수를 추가한다.
			m_TempGeomObject->m_mesh_numtvertex = Parsing_NumberInt();
		}
		break;

		/// MESH_TVERTLIST

		case TOKENR_MESH_TVERTLIST:
			//
			break;
		case TOKENR_MESH_TVERT:
		{
			// 버텍스의 인덱스가 나오는데 어차피 순서와 같으므로 버린다.
			Parsing_NumberInt();
			// 새로운 TVertex를 만들어서 벡터에 넣는다
			// COneTVertex를 새로 만들어서 벡터에 넣어준다.
			COneTVertex* _tempTVertex = new COneTVertex();
			_tempTVertex->m_u = Parsing_NumberFloat();
			_tempTVertex->m_v = Parsing_NumberFloat();
			_tempTVertex->m_w = Parsing_NumberFloat();

			m_TempGeomObject->m_mesh_tvertex.push_back(_tempTVertex);
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
		{
			m_TempGeomObject->m_mesh_tvfaces = Parsing_NumberInt();
		}
		break;
		case TOKENR_MESH_TFACE:
		{
			// FACE의 특정 인덱스에 해당하는 TVertexindex 값을 저장한다.
				// FACE 0 0, 2, 3 == TFACE 0 9, 11, 10
				// 0, 2, 3을 그려줄때, 9 11 10번 텍스쳐 좌표를 가져와서 그려준다.
			// 같은 인덱스의 페이스에 페이스 노말을 넣어준다.
			int _tempIdx = Parsing_NumberInt();
			m_TempGeomObject->m_meshface[_tempIdx]->m_TFace[0] = Parsing_NumberInt();
			m_TempGeomObject->m_meshface[_tempIdx]->m_TFace[1] = Parsing_NumberInt();
			m_TempGeomObject->m_meshface[_tempIdx]->m_TFace[2] = Parsing_NumberInt();
		}
		break;

		// SHAPE OBJ
		case TOKENR_SHAPEOBJECT:
		{
			// Mesh는 없지만 정보는 가지고 있어야하기 때문에..
			Create_onemesh_to_list();

			m_parsingmode = eParsingmode::eShape;
			// shape오브젝트는 Geom정보가 존재하지 않기 때문에(?) 관련된 부분을 0으로 해준다.
			m_TempGeomObject->m_mesh_numvertex = 0;
			m_TempGeomObject->m_mesh_numfaces = 0;
		}
		break;

		case TOKEND_END:
			// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

			//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
			TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
			return;

			break;

			/// 위의 아무것도 해당하지 않을때
		default:
			// 아무것도 하지 않는다.
			break;

		}	// switch()


		///-----------------------------------------------
		/// 안전 코드.
		i++;
		if (i > 1000000)
		{
			// 루프를 1000000번이상이나 돌 이유가 없다. (데이터가 100000개가 아닌이상)
			// 만약 1000000이상 돌았다면 확실히 뭔가 문제가 있는 것이므로
			TRACE("루프를 백만번 돌았습니다!");
			return;
		}
		/// 안전 코드.
		///-----------------------------------------------

	}		// while()

	// 여기까지 왔다면 while()을 벗어났다는 것이고 그 말은
	// 괄호를 닫았다는 것이므로
	// 리턴하자 (재귀함수)

	return;
}


///----------------------------------------------------------------------
/// parsing을 위한 단위별 함수들
///----------------------------------------------------------------------

// long을 읽어서 리턴해준다.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ※m_TokenString ( char[255] ) 이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}


// 3개의 Float를 벡터 하나로
Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// 스태틱 변수의 레퍼런스보다는 값 전달을 하자.
}

Vector3 CASEParser::Parsing_NumberVector3Mat()
{
	LONG				token;
	Vector3			tempVector3;

	// 기본적으로 위의 함수와 같으나 y, z축을 바꾸지 않는다.
		// 얘는 rgb 값이기 때문
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);

	return			tempVector3;
}

///--------------------------------------------------
/// 내부에서 뭔가를 생성, 리스트에 넣는다
///--------------------------------------------------
// 메시를 하나 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_onemesh_to_list()
{
	GeomObject* temp = new GeomObject;
	m_TempGeomObject = temp;
	m_TempGeomObject->m_scenedata = m_scenedata;		// 클래스간 값 복사
	m_GeomObjList.push_back(m_TempGeomObject);
}

// 메트리얼 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// 애니메이션데이터 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// 정점 하나를..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_TempGeomObject->m_meshvertex.push_back(temp);
}

















